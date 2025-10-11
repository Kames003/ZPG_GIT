#pragma once

const float plane[] = {
    // Position              // Normal
    -50.0f, 0.0f, -50.0f,   0.0f, 1.0f, 0.0f,  
     50.0f, 0.0f, -50.0f,   0.0f, 1.0f, 0.0f,  
     50.0f, 0.0f,  50.0f,   0.0f, 1.0f, 0.0f,  
    
    -50.0f, 0.0f, -50.0f,   0.0f, 1.0f, 0.0f,  
     50.0f, 0.0f,  50.0f,   0.0f, 1.0f, 0.0f,  
    -50.0f, 0.0f,  50.0f,   0.0f, 1.0f, 0.0f   
};
const int planeVertexCount = 6;  // 6 vrcholov = 2 trojuholniky