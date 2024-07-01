#include <GLFW/glfw3.h>
#include <math.h>

void hsv2rgb(float h, float s, float v, float *r, float *g, float *b) {
	h = h / 360;
	s = s / 100;
	v = v / 100;
	
	int i = floor(h * 6);
	float f = h * 6 - i;
	float p = v * (1 - s);
	float q = v * (1 - f * s);
	float t = v * (1 - (1 - f) * s);
	
	switch (i % 6) {
		case 0: *r = v, *g = t, *b = p; break;
		case 1: *r = q, *g = v, *b = p; break;
		case 2: *r = p, *g = v, *b = t; break;
		case 3: *r = p, *g = q, *b = v; break;
		case 4: *r = t, *g = p, *b = v; break;
		case 5: *r = v, *g = p, *b = q; break;
	}
}

int main(void)
{
    /* Initialize the library */
    if (!glfwInit())
        return -1;

    /* Create a windowed mode window and its OpenGL context */
    GLFWwindow *window = glfwCreateWindow(640, 480, "Hello World", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        return -1;
    }

    /* Make the window's context current */
    glfwMakeContextCurrent(window);

    float hue = 0;

    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window))
    {
        hue = hue < 359 ? hue + 1 : 0;

        float r, g, b;
        hsv2rgb(hue, 50, 100, &r, &g, &b);
        glClearColor(r, g, b, 1.f);
        glClear(GL_COLOR_BUFFER_BIT);

        /* Swap front and back buffers */
        glfwSwapBuffers(window);

        /* Poll for and process events */
        glfwPollEvents();
    }

    glfwTerminate();

    return 0;
}