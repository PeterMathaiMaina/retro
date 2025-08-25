#include "Camera.h"
#include "../../third_party/glm/gtc/matrix_transform.hpp"
#include "../../third_party/glm/gtx/quaternion.hpp"

void Camera::Update()
{
    // Update orientation vectors from quaternion
    m_forward   = glm::rotate(m_rotationQ, glm::vec3(0.0f, 0.0f, -1.0f));
    m_right     = glm::rotate(m_rotationQ, glm::vec3(1.0f, 0.0f, 0.0f));
    m_up        = glm::rotate(m_rotationQ, glm::vec3(0.0f, 1.0f, 0.0f));
    m_forwardXZ = glm::normalize(glm::vec3(m_forward.x, 0.0f, m_forward.z));

    // Build view matrix
    m_viewMatrix = glm::lookAt(m_position, m_position + m_forward, m_up);
    m_inverseViewMatrix = glm::inverse(m_viewMatrix);
}

void Camera::SetPosition(glm::vec3 position)
{
    m_position = position;
    Update();
}

void Camera::ProcessMouseMovement(double xoffset, double yoffset) {
    const float sensitivity = 0.1f;
    xoffset *= sensitivity;
    yoffset *= sensitivity;

    m_rotation.y -= xoffset; // yaw
    m_rotation.x += yoffset; // pitch

    // constrain pitch
    if (m_rotation.x > 89.0f) m_rotation.x = 89.0f;
    if (m_rotation.x < -89.0f) m_rotation.x = -89.0f;

    // update quaternion & matrices
    m_rotationQ = glm::quat(glm::radians(m_rotation));
    Update();
}


void Camera::SetEulerPosition(glm::vec3 rotation)
{
    m_rotation = rotation;
    m_rotationQ = glm::quat(rotation);
    Update();
}

void Camera::Orbit(float pitchOffset, float yawOffset)
{
    m_rotation.x += pitchOffset;
    m_rotation.y += yawOffset;

    if (m_rotation.x < m_minPitch) m_rotation.x = m_minPitch;
    if (m_rotation.x > m_maxPitch) m_rotation.x = m_maxPitch;

    m_rotationQ = glm::quat(m_rotation);
    Update();
}

void Camera::AddPitch(float value)
{
    Orbit(value, 0.0f);
}

void Camera::AddYaw(float value)
{
    Orbit(0.0f, value);
}

void Camera::AddHeight(float value)
{
    m_position.y += value;
    Update();
}

void Camera::SetMinPitch(float value) { m_minPitch = value; }
void Camera::SetMaxPitch(float value) { m_maxPitch = value; }

const glm::mat4& Camera::GetViewMatrix() const { return m_viewMatrix; }
const glm::mat4& Camera::GetInverseViewMatrix() const { return m_inverseViewMatrix; }
const glm::vec3& Camera::GetPosition() const { return m_position; }
const glm::vec3& Camera::GetEulerRotation() const { return m_rotation; }
const glm::quat& Camera::GetQuaternionRotation() const { return m_rotationQ; }
const glm::vec3& Camera::GetForward() const { return m_forward; }
const glm::vec3& Camera::GetUp() const { return m_up; }
const glm::vec3& Camera::GetRight() const { return m_right; }
const glm::vec3 Camera::GetForwardXZ() const { return m_forwardXZ; }
