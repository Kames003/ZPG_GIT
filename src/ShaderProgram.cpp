#include "ShaderProgram.h"
#include "Shader.h"
#include "Camera.h"
#include "Light.h"
#include <glm/gtc/type_ptr.hpp>
#include <stdio.h>
#include <string>

ShaderProgram::ShaderProgram(const std::vector<Shader*>& shaders)
{
    programID = glCreateProgram();

    for (auto shader : shaders) {
        shader->attachShader(programID);
    }

    glLinkProgram(programID);

    GLint status;
    glGetProgramiv(programID, GL_LINK_STATUS, &status);
    if (status == GL_FALSE) {
        GLint infoLogLength;
        glGetProgramiv(programID, GL_INFO_LOG_LENGTH, &infoLogLength);
        GLchar* strInfoLog = new GLchar[infoLogLength + 1];
        glGetProgramInfoLog(programID, infoLogLength, NULL, strInfoLog);
        fprintf(stderr, "Linker failure: %s\n", strInfoLog);
        delete[] strInfoLog;
    }
}

ShaderProgram::~ShaderProgram()
{
    if (programID != 0) {
        glDeleteProgram(programID);
    }
}

void ShaderProgram::use()
{
    glUseProgram(programID);
}

// ========================================
// UNIFORM SETTERS
// ========================================

void ShaderProgram::setUniform(const char* name, int value)
{
    GLint location = glGetUniformLocation(programID, name);
    glUniform1i(location, value);
}

void ShaderProgram::setUniform(const char* name, float value)
{
    GLint location = glGetUniformLocation(programID, name);
    glUniform1f(location, value);
}

void ShaderProgram::setUniform(const char* name, const glm::vec3& value)
{
    GLint location = glGetUniformLocation(programID, name);
    glUniform3fv(location, 1, glm::value_ptr(value));
}

void ShaderProgram::setUniform(const char* name, const glm::vec4& value)
{
    GLint location = glGetUniformLocation(programID, name);
    glUniform4fv(location, 1, glm::value_ptr(value));
}

void ShaderProgram::setUniform(const char* name, const glm::mat4& value)
{
    GLint location = glGetUniformLocation(programID, name);
    glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(value));
}

// ========================================
// SVETLÁ - UPRAVENÉ METÓDY (BOD 1 & 2)
// ========================================

/**
 * Pošle všetky svetlá do shadera (podľa učiteľa)
 *
 * Učiteľ (6:10): "Vy užijete ich výpočtu toho fondová osvetovacího modelu.
 * To, čo je tam ako taková malýka odlišnosť, je, že když to posílate
 * v tom pole na konkrétny prvek, tak to tam musíte takto napsať."
 */
void ShaderProgram::setLights(const std::vector<Light*>& lights)
{
    use();

    // Počet svetiel (ZMENENÉ z 4 na 8)
    int lightCount = static_cast<int>(lights.size());
    if (lightCount > 8) {  // ← MAX_LIGHTS = 8
        printf("WARNING: Too many lights (%d), max is 8\n", lightCount);
        lightCount = 8;
    }

    setUniform("numberOfLights", lightCount);

    // Pošli každé svetlo na svoju pozíciu v poli
    for (int i = 0; i < lightCount; i++) {
        setLight(i, lights[i]);
    }
}

/**
 * Pošle jedno svetlo na konkrétnu pozíciu v poli
 *
 * KRITICKÁ ZMENA: Teraz posielame ambient, diffuse, specular namiesto color!
 */

/**
 * Pošle jedno svetlo na konkrétnu pozíciu v poli
 *
 * KRITICKÁ ZMENA: Ambient sa posiela LEN pre index 0!
 * Učiteľ (27:00): "Observer sa musí rozhodnúť, čo by delať"
 */
void ShaderProgram::setLight(int index, Light* light)
{
    if (!light) return;

    use();

    // Vytvor názvy uniformov: "lights[0].position", "lights[0].ambient", ...
    std::string baseName = "lights[" + std::to_string(index) + "].";

    // ========================================
    // POSITION (vec4)
    // ========================================
    std::string posName = baseName + "position";
    glm::vec3 pos = light->getPosition();
    setUniform(posName.c_str(), glm::vec4(pos, 1.0f));

    // ========================================
    // AMBIENT - LEN PRE PRVÉ SVETLO (index 0)!
    // ========================================
    glm::vec3 lightColor = light->getColor();

    if (index == 0) {
        // Ambient - iba pre prvé svetlo (20% z farby)
        std::string ambientName = baseName + "ambient";
        setUniform(ambientName.c_str(), lightColor * 0.2f);
    } else {
        // Ostatné svetlá majú ambient = 0
        std::string ambientName = baseName + "ambient";
        setUniform(ambientName.c_str(), glm::vec3(0.0f));
    }

    // ========================================
    // DIFFUSE a SPECULAR - PRE VŠETKY SVETLÁ
    // ========================================
    std::string diffuseName = baseName + "diffuse";
    setUniform(diffuseName.c_str(), lightColor);

    std::string specularName = baseName + "specular";
    setUniform(specularName.c_str(), lightColor);

    // ========================================
    // INTENSITY (float)
    // ========================================
    std::string intensityName = baseName + "intensity";
    setUniform(intensityName.c_str(), light->getIntensity());
}

/**
 * Update jedno svetlo (keď sa zmení cez notifikáciu)
 *
 * Učiteľ (7:33): "Keď sa mi to svetlo zmení,
 * tak ja ho musím aktualizovať v pozícii"
 */
void ShaderProgram::updateLightUniforms(Light* light)
{
    // Použije shaderIndex, ktorý má svetlo priradený
    int index = light->getShaderIndex();

    if (index >= 0 && index < 8) {  // ← MAX_LIGHTS = 8
        setLight(index, light);
    }
}

// ========================================
// OBSERVER PATTERN
// ========================================

void ShaderProgram::notify(Subject* subject)
{
    // Kamera
    Camera* camera = dynamic_cast<Camera*>(subject);
    if (camera) {
        updateCameraUniforms(camera);
        return;
    }

    // Svetlo - FUNGUJE cez observer pattern!
    Light* light = dynamic_cast<Light*>(subject);
    if (light) {
        updateLightUniforms(light);
        return;
    }
}

void ShaderProgram::updateCameraUniforms(Camera* camera)
{
    use();
    setUniform("viewMatrix", camera->getViewMatrix());
    setUniform("viewPosition", camera->getPosition());
    setUniform("projectionMatrix", camera->getProjectionMatrix());
}