#version 150

smooth in vec2 TexCoord;

uniform sampler2D tex;

out vec4 outColor;


// Camera
uniform vec2 m_resolution;
uniform vec3 m_camUp;
uniform vec3 m_camRight;
uniform vec3 m_camForward;
uniform vec3 m_eye;
uniform float m_focalLength;
uniform float m_zNear;
uniform float m_zFar;
uniform float m_aspectRatio;

// Raymarch parameters
uniform int m_rmSteps; // Max steps
uniform float m_rmEpsilon; // Distance threshold

// Scene
uniform vec4 m_skyColor;
uniform vec4 m_ambient;
uniform vec3 m_light0Position;
uniform vec4 m_light0Color;
uniform vec3 m_light1Position;
uniform vec4 m_light1Color;

//Debug options
uniform int m_MSAA;
uniform int m_AO;

uniform vec3 m_navAgentPos;

uniform int m_texSize;

// Rotates a point t radians around the y-axis
vec3 rotateY(vec3 v, float t)
{
	float cost = cos(t); float sint = sin(t);
	return vec3(v.x * cost + v.z * sint, v.y, -v.x * sint + v.z * cost);
}

// Rotates a point t radians around the x-axis
vec3 rotateX(vec3 v, float t)
{
	float cost = cos(t); float sint = sin(t);
	return vec3(v.x, v.y * cost - v.z * sint, v.y * sint + v.z * cost);
}

// Maps x from the range [minX, maxX] to the range [minY, maxY]
// The function does not clamp the result, as it may be useful
float mapTo(float x, float minX, float maxX, float minY, float maxY)
{
	float a = (maxY - minY) / (maxX - minX);
	float b = minY - a * minX;
	return a * x + b;
}

// Returns the signed distance to a sphere at the origin
float sdSphere(vec3 p, float radius)
{
	return length(p) - radius;
}

// Returns the unsigned distance estimate to a box at the origin of the given size
float udBox(vec3 p, vec3 size)
{
	return length(max(abs(p) - size, vec3(0.0f)));
}

// Returns the signed distance estimate to a box at the origin of the given size
float sdBox(vec3 p, vec3 size)
{
	vec3 d = abs(p) - size;
	return min(max(d.x, max(d.y, d.z)), 0.0f) + udBox(p, size);
}

// Subtracts d1 from d0, assuming d1 is a signed distance
float opSubtract(float d0, float d1)
{
	return max(d0, -d1);
}

// Calcs intersection and exit distances, and normal at intersection
//
// The box is axis aligned and at the origin, but has any size.
// For arbirarily oriented and located boxes, simply transform
// the ray accordingly and reverse the transformation for the
// returning normal(without translation)
//
vec2 boxIntersection( vec3 ro, vec3 rd, vec3 boxSize, out vec3 outNormal ) 
{
    vec3 m = 1.0/rd;
    vec3 n = m*ro;
    vec3 k = abs(m)*boxSize;
	
    vec3 t1 = -n - k;
    vec3 t2 = -n + k;

    float tN = max( max( t1.x, t1.y ), t1.z );
    float tF = min( min( t2.x, t2.y ), t2.z );
	
    if( tN > tF || tF < 0.0) return vec2(-1.0); // no intersection
    
    outNormal = -sign(rd)*step(t1.yzx,t1.xyz)*step(t1.zxy,t1.xyz);

    return vec2( tN, tF );
}

//Check if ray intersects plane
float rayPlaneIntersect(vec3 ro, vec3 rd, vec3 n, vec3 c)
{
	float denom = dot(n, rd);
	if(abs(denom) > m_rmEpsilon)
	{
		float t = dot((c - ro), n) / denom;
		if(t >= m_rmEpsilon) return t;
	}
	return 0.0f;
}

bool sdFromTex(vec3 p, vec2 o, inout float dist)
{
	if(texture(tex, (p.xz + o) / m_texSize).r > 0.5f)
	{
		dist = min(dist, sdBox(p - vec3(floor(p.x) + o.x + 0.5f, 0.0f, floor(p.z) + o.y + 0.5f), vec3(0.5f)));
		return true;
	}
	else
	{
		return false;
	}
}

// Defines the distance field for the scene
float distScene(vec3 ro, vec3 rd, float t)
{
	//p.xz = mod(p.xz, 1.0f);
	vec3 p = ro + rd * t;
	float closest = 100;
	vec3 flooredP = vec3(floor(p.x), p.y, floor(p.z));
	if(sdFromTex(p, vec2(0), closest))
	{
		return min(closest, sdSphere(p - m_navAgentPos, 0.25f));	
	}
	else
	{
		closest = 100;
		if(rd.x > 0)
		{
			sdFromTex(p, vec2(1, 0), closest);
			if(rd.z > 0)
			{
				if(!sdFromTex(p, vec2(0, 1), closest))
					sdFromTex(p, vec2(1, 1), closest);
			}
			else
			{
				if(!sdFromTex(p, vec2(0, -1), closest))
					sdFromTex(p, vec2(1, -1), closest);			
			}
		}
		else
		{
			sdFromTex(p, vec2(-1, 0), closest);
			
			if(rd.z > 0)
			{
				if(!sdFromTex(p, vec2(0, 1), closest))
					sdFromTex(p, vec2(-1, 1), closest);
			}
			else
			{
				if(!sdFromTex(p, vec2(0, -1), closest))
					sdFromTex(p, vec2(-1, -1), closest);
			}
		}
		return min(closest, sdSphere(p - m_navAgentPos, 0.25f));
	}
	return min(closest, sdSphere(p - m_navAgentPos, 0.25f));
	//return sdBox(p - vec3(floor(p.x) + 0.5f, 0.0f, floor(p.z) + 0.5f), vec3(0.45f));


	//return sdSphere(p - vec3(0.0f, -0.25f, 0.0f), 0.25f);

	// p = rotateY(p, 0.5f * p.y);
	// float d1 = sdBox(p - vec3(0, 0.5, 0), vec3(0.5, 1.0, 0.5));
	// float d2 = sdBox(p, vec3(2.0, 0.3, 0.25));
	// return opSubtract(d1, d2);
}



// Finds the closest intersecting object along the ray at origin ro, and direction rd.
// i: step count
// t: distance traveled by the ray
void raymarch(vec3 ro, vec3 rd, out int i, out float t)
{
	t = rayPlaneIntersect(ro, rd, vec3(0, 1, 0), vec3(0, 1, 0));
	
	for(i = 0; i < m_rmSteps; ++i)
	{
		float dist = distScene(ro, rd, t);

		// We make epsilon proportional to t so that we drop accuracy the further into the scene we get
		// We also drop the ray as soon as it leaves the clipping volume as defined by m_zFar
		if(dist < m_rmEpsilon * t * 2.0f || t > m_zFar)
			break;
		t += dist;
	}
}

// Returns a value between [0, 1] depending on how visible p0 is from p1
// k: denotes the soft-shadow strength
// See http://www.iquilezles.org/www/articles/rmshadows/rmshadows.htm
float getVisibility(vec3 p0, vec3 p1, float k)
{
	vec3 rd = normalize(p1 - p0);
	float t = 10.0f * m_rmEpsilon;
	float maxt = length(p1 - p0);
	float f = 1.0f;
	while(t < maxt)
	{
		float d = distScene(p0, rd, t);

		// A surface was hit before we reached p1
		if(d < m_rmEpsilon)
			return 0.0f;

		// Penumbra factor
		f = min(f, k * d / t);

		t += d;
	}

	return f;
}

// Approximates the (normalized) gradient of the distance function at the given point.
// If p is near a surface, the function will approximate the surface normal.
vec3 getNormal(vec3 ro, vec3 rd, float t)
{
	float h = 0.0001f;
	vec3 p = ro + rd * t;
	return normalize(vec3(
		distScene(ro + vec3(h, 0, 0), rd, t) - distScene(ro - vec3(h, 0, 0), rd, t),
		distScene(ro + vec3(0, h, 0), rd, t) - distScene(ro - vec3(0, h, 0), rd, t),
		distScene(ro + vec3(0, 0, h), rd, t) - distScene(ro - vec3(0, 0, h), rd, t)));
}

// Calculate the light intensity with soft shadows
// p: point on surface
// lightPos: position of the light source
// lightColor: the radiance of the light source
// returns: the color of the point
vec4 getShading(vec3 p, vec3 normal, vec3 lightPos, vec4 lightColor)
{
	float intensity = 0.0f;
	float vis = getVisibility(p, lightPos, 16);
	if(vis > 0.0f)
	{
		vec3 lightDirection = normalize(lightPos - p);
		intensity = clamp(dot(normal, lightDirection), 0, 1) * vis;
	}


	return lightColor * intensity + m_ambient * (1.0f - intensity);
}

// Compute an ambient occlusion factor
// p: point on surface
// n: normal of the surface at p
// returns: a value clamped to [0, 1], where 0 means there were no other surfaces around the point,
// and 1 means that the point is occluded by other surfaces.
float ambientOcclusion(vec3 p, vec3 n)
{
	float stepSize = 0.01f;
	float t = stepSize;
	float oc = 0.0f;
	for(int i = 0; i < 10; ++i)
	{
		float d = distScene(p, n, t);
		oc += t - d; // Actual distance to surface - distance field value
		t += stepSize;
	}

	return clamp(oc, 0, 1);
}

// Create a checkboard texture
vec4 getFloorTexture(vec3 p)
{
	//vec2 m = mod(p.xz, 2.0f) - vec2(1.0f);
	//return m.x * m.y > 0.0f ? vec4(0.1f) : vec4(1.0f);
	vec4 col = vec4(0.1f);
	if(texture(tex, p.xz / m_texSize).r > 0f)
	{
		col.r = 0.5f;
	}
	return col;
}

// To improve performance we raytrace the floor
// n: floor normal
// o: floor position
float raytraceFloor(vec3 ro, vec3 rd, vec3 n, vec3 o)
{
	return dot(o - ro, n) / dot(rd, n);
}

vec4 computeColor(vec3 ro, vec3 rd)
{
	float t0;
	int i;
	raymarch(ro, rd, i, t0);

	vec3 floorNormal = vec3(0, 1, 0);
	float t1 = raytraceFloor(ro, rd, floorNormal, vec3(0, -0.5, 0));
	
	vec4 color = vec4(1);
	vec3 p; // Surface point
	vec3 normal; // Surface normal
	float t; // Distance traveled by ray from eye
	vec4 surfTexture = vec4(1.0); // Surface texture
	if(t1 < t0 && t1 >= m_zNear && t1 <= m_zFar) // The floor was closest
	{
		t = t1;
		p = ro + rd * t1;
		normal = floorNormal;
		surfTexture = getFloorTexture(p);
	}
	else if(i < m_rmSteps && t0 >= m_zNear && t0 <= m_zFar) // Raymarching hit a surface
	{
		t = t0;
		p = ro + rd * t0;
		normal = getNormal(ro, rd, t0);
	}
	else
	{
		return m_skyColor;
	}

	
	float z = mapTo(t, m_zNear, m_zFar, 1, 0); // Map depth to [0, 1]

	// Color based on depth
	//color = vec4(1.0f) * z;

	// Diffuse lighting
	color *= surfTexture * (
		getShading(p, normal, m_light0Position, m_light0Color) +
		getShading(p, normal, m_light1Position, m_light1Color)
	) / 2.0f;
	// Color based on surface normal
	//color = vec4(abs(normal), 1.0);

	if(m_AO > 0)
	{
		// Blend in ambient occlusion factor
		float ao = ambientOcclusion(p, normal);
		color = color * (1.0f - ao);
	}

	// Blend the background color based on the distance from the camera
	float zSqrd = z * z;
	color = mix(m_skyColor, color, zSqrd * (3.0f - 2.0f * z)); // Fog

	return color;

}

void main()
{
	vec2 hps = vec2(1.0) / (m_resolution * 2.0);
	vec3 ro = m_eye;
	vec3 rd = normalize(m_camForward * m_focalLength + m_camRight * TexCoord.x * m_aspectRatio + m_camUp * TexCoord.y);

	vec4 color;
	if(m_MSAA == 0)
	{
		 color = computeColor(ro, rd);
	}
	else
	{
	// 4xAA
		vec3 rd0 = normalize(m_camForward * m_focalLength + m_camRight * (TexCoord.x - hps.x) * m_aspectRatio + m_camUp * TexCoord.y);
		vec3 rd1 = normalize(m_camForward * m_focalLength + m_camRight * (TexCoord.x + hps.x) * m_aspectRatio + m_camUp * TexCoord.y);
		vec3 rd2 = normalize(m_camForward * m_focalLength + m_camRight * TexCoord.x * m_aspectRatio + m_camUp * (TexCoord.y - hps.y));
		vec3 rd3 = normalize(m_camForward * m_focalLength + m_camRight * TexCoord.x * m_aspectRatio + m_camUp * (TexCoord.y + hps.y));
	
		color = (computeColor(ro, rd0) + computeColor(ro, rd1) + computeColor(ro, rd2) + computeColor(ro, rd3)) / 4.0;
	}
	outColor = vec4(color.xyz, 1.0f);
}

//void main()
//{
//	outColor = vec4(texture(tex, TexCoord));
//}