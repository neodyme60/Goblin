#include "GoblinVector.h"

namespace Goblin {
    const Vector2 Vector2::UnitX = Vector2(1.0f, 0.0f);
    const Vector2 Vector2::UnitY = Vector2(0.0f, 1.0f);

    const Vector3 Vector3::UnitX = Vector3(1.0f, 0.0f, 0.0f);
    const Vector3 Vector3::UnitY = Vector3(0.0f, 1.0f, 0.0f);
    const Vector3 Vector3::UnitZ = Vector3(0.0f, 0.0f, 1.0f);

    const Vector4 Vector4::UnitX = Vector4(1.0f, 0.0f, 0.0f, 0.0f);
    const Vector4 Vector4::UnitY = Vector4(0.0f, 1.0f, 0.0f, 0.0f);
    const Vector4 Vector4::UnitZ = Vector4(0.0f, 0.0f, 1.0f, 0.0f);
    const Vector4 Vector4::UnitW = Vector4(0.0f, 0.0f, 0.0f, 1.0f);
}
