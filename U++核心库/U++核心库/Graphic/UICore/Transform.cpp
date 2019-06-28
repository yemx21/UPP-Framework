#include "Transform.h"
#include "MatrixTransform.h"

using namespace System::UI;
using namespace System::UI::Media;

Transform::Transform()
{

}

Transform::~Transform()
{

}

TransformPtr Transform::Identity()
{
	return std::dynamic_pointer_cast<Transform>(std::make_shared<MatrixTransform>(Matrix::Identity()));
}
