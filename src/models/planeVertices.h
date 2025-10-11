#pragma once

// Plane (ground) - 100x100 units, centered at origin
const float plane[] = {
    // Position              // Normal
    -50.0f, 0.0f, -50.0f,   0.0f, 1.0f, 0.0f,  // Bottom-left
     50.0f, 0.0f, -50.0f,   0.0f, 1.0f, 0.0f,  // Bottom-right
     50.0f, 0.0f,  50.0f,   0.0f, 1.0f, 0.0f,  // Top-right
    
    -50.0f, 0.0f, -50.0f,   0.0f, 1.0f, 0.0f,  // Bottom-left
     50.0f, 0.0f,  50.0f,   0.0f, 1.0f, 0.0f,  // Top-right
    -50.0f, 0.0f,  50.0f,   0.0f, 1.0f, 0.0f   // Top-left
};

const int planeVertexCount = 6;