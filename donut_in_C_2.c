/**
 * @file donut_in_C.c
 * @author your name (you@domain.com)
 * @brief 
 * @version 0.1
 * @date 2024-09-08
 * @soure https://www.a1k0n.net/2011/07/20/donut-math.html
 * @copyright Copyright (c) 2024
 * 
 */

#include <stdio.h>
#include <math.h>
#include <string.h>

#define SCREEN_WIDTH        1760
#define SCREEN_HEIGHT       80
#define TWO_PI              6.28F
#define THETA_SPACING       0.07F
#define PHI_SPACING         0.02F
#define HEIGHT_CENTER       40      // Assuming height is 80 ch
#define WIDTH_CENTER        12      // Assuming width is 24 ch
#define SCALING_FACTOR_X    30      // Spreding the donut horzontally
#define SCALING_FACTOR_Y    15      // Spreding the donut vertically

const float R1 = 1;
const float R2 = 2;
const float K2 = 5;

void print_donut()
{
    float rotation_angle_x = 0, rotation_angle_z = 0; // rotation angle arount X and Z axes
    float torus_theta = 0, torus_phi = 0; // angles for points on torus
    
    int screen_index = 0;

    float cos_theta = 0,  sin_theta = 0, cos_phi = 0, sin_phi = 0;
    float sin_rotation_x = 0, cos_rotation_x = 0, sin_rotation_z = 0, cos_rotation_z = 0;

    // float x = 0, y = 0, z = 0;
    float circle_x = 0, circle_y = 0, inv_z = 0;
    float projected_x = 0;
    int screen_x = 0, screen_y = 0, screen_pos = 0;
    
    int luminance_index = 0;

    float depth_buffer[SCREEN_WIDTH]; // Z-buffer for depth info
    char output_buffer[SCREEN_WIDTH]; // Character buffer for ASCII output
    printf("\x1b[2J"); // Clear the screen

    for(;;)
    {
        memset(output_buffer, ' ', sizeof(output_buffer)); // Fill the buffer with spaces
        memset(depth_buffer, 0, sizeof(depth_buffer)); // Clear the depth buffer
        
        // Loop over the torus points
        // Theta goes around the cross-sectional circle of a torus
        for(torus_theta = 0; torus_theta < TWO_PI; torus_theta += THETA_SPACING)
        {
            // Phi goes around the center of revolution of a torus
            for(torus_phi = 0; torus_phi < TWO_PI; torus_phi += PHI_SPACING)
            {
                // Precompute sines and cosines for performance
                cos_phi = cos(torus_phi);
                sin_phi = sin(torus_phi);
                cos_theta = cos(torus_theta);
                sin_theta = sin(torus_theta);

                // Rotation around X-axis
                sin_rotation_x = sin(rotation_angle_x);
                cos_rotation_x = cos(rotation_angle_x);
                // Rotation around Z-axis
                sin_rotation_z = sin(rotation_angle_z);
                cos_rotation_z = cos(rotation_angle_z);

                // Calculate the 3D coordinates of the donut
                circle_x    = cos_theta + R2;  // Offset torus radius
                circle_y    = sin_phi;
                inv_z = 1 / (sin_phi * circle_x * sin_rotation_x + sin_theta * cos_rotation_x + 5);
                
                projected_x = cos_phi * circle_x * cos_rotation_x - sin_theta * sin_rotation_x;
                // Assuming the screen width is 80 ch and height is 24 ch (40, 12) is center of donut
                screen_x = HEIGHT_CENTER + SCALING_FACTOR_X * inv_z * (projected_x * cos_rotation_z - circle_y * sin_rotation_z);
                screen_y = WIDTH_CENTER + SCALING_FACTOR_Y * inv_z * (projected_x * sin_rotation_z + circle_y * cos_rotation_z);

                screen_pos = screen_x + SCREEN_HEIGHT * screen_y;

                luminance_index = 8 * ((sin_theta * sin_rotation_x - sin_phi * cos_theta * cos_rotation_x) * cos_rotation_z 
                                           - sin_phi * cos_theta * sin_rotation_x - sin_theta * cos_rotation_x 
                                           - cos_phi * cos_theta * sin_rotation_z);
                // Check if pixel is within screen bounds and update the depth buffer
                if( screen_y > 0 && screen_y < 22 && screen_x > 0 && screen_x < 80 && inv_z > depth_buffer[screen_pos])
                {
                    depth_buffer[screen_pos] = inv_z;
                    output_buffer[screen_pos] = ".,-~:;=!*#$@"[luminance_index > 0 ? luminance_index : 0];
                }
            }
        }
        printf("\x1b[H"); // Reset cursor to the top left
        for(screen_index = 0; screen_index < SCREEN_WIDTH; screen_index++)
        {
            putchar(screen_index % SCREEN_HEIGHT ? output_buffer[screen_index] : 10);
        }

        rotation_angle_x += 0.04; // Increment rotation angle X-axis
        rotation_angle_z += 0.02; // Increment rotation angle Z-axis
    }
}


int main(void)
{
    print_donut();
    
    return 0;
}