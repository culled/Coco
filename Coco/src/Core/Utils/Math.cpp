#include "ccpch.h"
#include "Math.h"

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/matrix_decompose.hpp>

namespace Coco {
	namespace Math {

}
	bool Math::DecomposeTransform(const glm::mat4& transform, glm::vec3& outPosition, glm::vec3& outRotation, glm::vec3& outScale)
	{
		//From glm::decompose in matrix_decompose.inl

		using namespace glm;
		
		mat4 LocalMatrix(transform);

		//Normalize the matrix
		if (epsilonEqual(LocalMatrix[3][3], 0.0f, epsilon<float>()))
			return false;

		for (int i = 0; i < 4; ++i)
			for (int j = 0; j < 4; ++j)
				LocalMatrix[i][j] /= LocalMatrix[3][3];

		//Get the position
		outPosition = vec3(LocalMatrix[3]);
		LocalMatrix[3] = vec4(0.0f, 0.0f, 0.0f, LocalMatrix[3].w);

		//vec3 Row[3], Pdum3;

		// Now get scale and shear.
		//for (length_t i = 0; i < 3; ++i)
		//	for (length_t j = 0; j < 3; ++j)
		//		Row[i][j] = LocalMatrix[i][j];

		outScale.x = length(LocalMatrix[0]);// v3Length(Row[0]);
		outScale.y = length(LocalMatrix[1]);
		outScale.z = length(LocalMatrix[2]);

		outRotation.y = asin(-LocalMatrix[0][2]);
		if (cos(outRotation.y) != 0) {
			outRotation.x = atan2(LocalMatrix[1][2], LocalMatrix[2][2]);
			outRotation.z = atan2(LocalMatrix[0][1], LocalMatrix[0][0]);
		} else {
			outRotation.x = atan2(-LocalMatrix[2][0], LocalMatrix[1][1]);
			outRotation.z = 0;
		}

		return true;
	}
}