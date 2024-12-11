typedef struct {
	float x;
	float y;
} vector2;

typedef struct{
	float x;
	float y;
	float z;
} vector3;

typedef struct {
	float m[3][3];
} matrix3x3;

typedef struct {
	float m[4][4];
} matrix4x4;

vector2 vector2_add(vector2 a, vector2 b) {
	vector2 result;
	result.x = a.x + b.x;
	result.y = a.y + b.y;
	return result;
}

vector2 vector2_sub(vector2 a, vector2 b) {
	vector2 result;
	result.x = a.x - b.x;
	result.y = a.y - b.y;
	return result;
}

vector2 vector2_multiply_scalar(vector2 a, float b) {
	vector2 result;
	result.x = a.x * b;
	result.y = a.y * b;
	return result;
}

float vector2_dot(vector2 a, vector2 b) {
	return a.x * b.x + a.y * b.y;
}

vector2  vector2_magnitude(vector2 a) {
	vector2 result;
	result.x = sqrt(a.x * a.x + a.y * a.y);
	result.y = sqrt(a.x * a.x + a.y * a.y);
	return result;
}

vector2 vector2_normalize(vector2 a) {
	vector2 result;
	result.x = a.x / vector2_magnitude(a).x;
	result.y = a.y / vector2_magnitude(a).y;
	return result;
}

float vector2_distance(vector2 a, vector2 b) {
	return sqrt((a.x - b.x) * (a.x - b.x) + (a.y - b.y) * (a.y - b.y));
}

vector3 vector3_add(vector3 a, vector3 b) {
	vector3 result;
	result.x = a.x + b.x;
	result.y = a.y + b.y;
	result.z = a.z + b.z;
	return result;
}

vector3 vector3_sub(vector3 a, vector3 b) {
	vector3 result;
	result.x = a.x - b.x;
	result.y = a.y - b.y;
	result.z = a.z - b.z;
	return result;
}

vector3 vector3_multiply_scalar(vector3 a, float b) {
	vector3 result;
	result.x = a.x * b;
	result.y = a.y * b;
	result.z = a.z * b;
	return result;
}

float vector3_dot(vector3 a, vector3 b) {
	return a.x * b.x + a.y * b.y + a.z * b.z;
}

vector3 vector3_cross(vector3 a, vector3 b) {
	vector3 result;
	result.x = a.y * b.z - a.z * b.y;
	result.y = a.z * b.x - a.x * b.z;
	result.z = a.x * b.y - a.y * b.x;
	return result;
}

vector3 vector3_magnitude(vector3 a) {
	vector3 result;
	result.x = sqrt(a.x * a.x + a.y * a.y + a.z * a.z);
	result.y = sqrt(a.x * a.x + a.y * a.y + a.z * a.z);
	result.z = sqrt(a.x * a.x + a.y * a.y + a.z * a.z);
	return result;
}

vector3 vector3_normalize(vector3 a) {
	vector3 result;
	result.x = a.x / vector3_magnitude(a).x;
	result.y = a.y / vector3_magnitude(a).y;
	result.z = a.z / vector3_magnitude(a).z;
	return result;
}

float vector3_distance(vector3 a, vector3 b) {
	return sqrt((a.x - b.x) * (a.x - b.x) + (a.y - b.y) * (a.y - b.y) + (a.z - b.z) * (a.z - b.z));
}

matrix3x3 matrix3x3_identity() {
	matrix3x3 result;
	result.m[0][0] = 1.0f;
	result.m[0][1] = 0.0f;
	result.m[0][2] = 0.0f;
	result.m[1][0] = 0.0f;
	result.m[1][1] = 1.0f;
	result.m[1][2] = 0.0f;
	result.m[2][0] = 0.0f;
	result.m[2][1] = 0.0f;
	result.m[2][2] = 1.0f;
	return result;
}

matrix3x3 matrix3x3_multiply(matrix3x3 a, matrix3x3 b) {
	matrix3x3 result;
	for (int i = 0; i < 3; i++) {
		for (int j = 0; j < 3; j++) {
			result.m[i][j] = a.m[i][0] * b.m[0][j] + a.m[i][1] * b.m[1][j] + a.m[i][2] * b.m[2][j];
		}
	}
	return result;
}

matrix3x3 matrix3x3_translate(float x, float y) {
	matrix3x3 result;
	result.m[0][0] = 1.0f;
	result.m[0][1] = 0.0f;
	result.m[0][2] = x;
	result.m[1][0] = 0.0f;
	result.m[1][1] = 1.0f;
	result.m[1][2] = y;
	result.m[2][0] = 0.0f;
	result.m[2][1] = 0.0f;
	result.m[2][2] = 1.0f;
	return result;
}

matrix3x3 matrix3x3_rotation(float angle) {
	matrix3x3 result;
	result.m[0][0] = cos(angle);
	result.m[0][1] = -sin(angle);
	result.m[0][2] = 0.0f;
	result.m[1][0] = sin(angle);
	result.m[1][1] = cos(angle);
	result.m[1][2] = 0.0f;
	result.m[2][0] = 0.0f;
	result.m[2][1] = 0.0f;
	result.m[2][2] = 1.0f;
	return result;
}

matrix3x3 matrix3x3_scaling(float x, float y) {
	matrix3x3 result;
	result.m[0][0] = x;
	result.m[0][1] = 0.0f;
	result.m[0][2] = 0.0f;
	result.m[1][0] = 0.0f;
	result.m[1][1] = y;
	result.m[1][2] = 0.0f;
	result.m[2][0] = 0.0f;
	result.m[2][1] = 0.0f;
	result.m[2][2] = 1.0f;
	return result;
}

vector2 matrix3x3_transform_vector2(matrix3x3 m, vector2 v) {
	vector2 result;
	result.x = m.m[0][0] * v.x + m.m[0][1] * v.y + m.m[0][2];
	result.y = m.m[1][0] * v.x + m.m[1][1] * v.y + m.m[1][2];
	return result;
}

matrix4x4 matrix4x4_identity() {
	matrix4x4 result;
	result.m[0][0] = 1.0f;
	result.m[0][1] = 0.0f;
	result.m[0][2] = 0.0f;
	result.m[0][3] = 0.0f;
	result.m[1][0] = 0.0f;
	result.m[1][1] = 1.0f;
	result.m[1][2] = 0.0f;
	result.m[1][3] = 0.0f;
	result.m[2][0] = 0.0f;
	result.m[2][1] = 0.0f;
	result.m[2][2] = 1.0f;
	result.m[2][3] = 0.0f;
	result.m[3][0] = 0.0f;
	result.m[3][1] = 0.0f;
	result.m[3][2] = 0.0f;
	result.m[3][3] = 1.0f;
	return result;
}

matrix4x4 matrix4x4_multiply(matrix4x4 a, matrix4x4 b) {
	matrix4x4 result;
	for (int i = 0; i < 4; ++i) {
		for (int j = 0; j < 4; ++j) {
			result.m[i][j] = a.m[i][0] * b.m[0][j] + a.m[i][1] * b.m[1][j] + a.m[i][2] * b.m[2][j] + a.m[i][3] * b.m[3][j];
		}
	}
	return result;
}

matrix4x4 matrix4x4_translation(float tx, float ty, float tz) {
	matrix4x4 result = matrix4x4_identity();
	result.m[0][3] = tx;
	result.m[1][3] = ty;
	result.m[2][3] = tz;
	return result;
}

matrix4x4 matrix4x4_rotation_x(float angle) {
	float c = cos(angle);
	float s = sin(angle);
	matrix4x4 result = matrix4x4_identity();
	result.m[1][1] = c;
	result.m[1][2] = -s;
	result.m[2][1] = s;
	result.m[2][2] = c;
	return result;
}

matrix4x4 matrix4x4_rotation_y(float angle) {
	float c = cos(angle);
	float s = sin(angle);
	matrix4x4 result = matrix4x4_identity();
	result.m[0][0] = c;
	result.m[0][2] = s;
	result.m[2][0] = -s;
	result.m[2][2] = c;
	return result;
}

matrix4x4 matrix4x4_rotation_z(float angle) {
	float c = cos(angle);
	float s = sin(angle);
	matrix4x4 result = matrix4x4_identity();
	result.m[0][0] = c;
	result.m[0][1] = -s;
	result.m[1][0] = s;
	result.m[1][1] = c;
	return result;
}

matrix4x4 matrix4x4_scaling(float sx, float sy, float sz) {
	matrix4x4 result = matrix4x4_identity();
	result.m[0][0] = sx;
	result.m[1][1] = sy;
	result.m[2][2] = sz;
	return result;
}

vector3 matrix4x4_transform_vector3(matrix4x4 m, vector3 v) {
	vector3 result;
	result.x = m.m[0][0] * v.x + m.m[0][1] * v.y + m.m[0][2] * v.z + m.m[0][3];
	result.y = m.m[1][0] * v.x + m.m[1][1] * v.y + m.m[1][2] * v.z + m.m[1][3];
	result.z = m.m[2][0] * v.x + m.m[2][1] * v.y + m.m[2][2] * v.z + m.m[2][3];
	return result;
}
