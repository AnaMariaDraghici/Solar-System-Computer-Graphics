#include <GLFW/glfw3.h>
#include <GL/glut.h>
#include <unistd.h>
#include <math.h>
#define STB_IMAGE_IMPLEMENTATION
#include <stb/stb_image.h>


void drawSphere(double r, int lats, int longs, float angle, float tx, float ty, float tz, GLuint texture) {
    int i, j;
    glPushMatrix();
    glTranslatef(tx, ty, tz);
    glRotatef(angle, 0.0f, 1.0f, 0.0f);
    glBindTexture(GL_TEXTURE_2D, texture);
    for(i = 0; i <= lats; i++) {
        double lat0 = M_PI * (-0.5 + (double) (i - 1) / lats);
        double z0  = sin(lat0);
        double zr0 =  cos(lat0);

        double lat1 = M_PI * (-0.5 + (double) i / lats);
        double z1 = sin(lat1);
        double zr1 = cos(lat1);

        glBegin(GL_QUAD_STRIP);
        for(j = 0; j <= longs; j++) {
            double lng = 2 * M_PI * (double) (j - 1) / longs;
            double x = cos(lng);
            double y = sin(lng);

            glTexCoord2f((float) j / longs, (float) (i - 1) / lats);
            glVertex3f(r * x * zr0, r * y * zr0, r * z0);

            glTexCoord2f((float) j / longs, (float) i / lats);
            glVertex3f(r * x * zr1, r * y * zr1, r * z1);
        }
        glEnd();
    }
    glPopMatrix();
}

GLuint loadTexture(const char* textureFile) {
    int width, height, channels;
    stbi_uc* data = stbi_load(textureFile, &width, &height, &channels, STBI_rgb);
    if (!data) {
        fprintf(stderr, "Error loading texture file %s\n", textureFile);
        exit(1);
    }

    GLuint texture;
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
    stbi_image_free(data);

    if (texture == 0) {
        printf("Error loading texture\n");
        return -1;
    }
    return texture;
}

float rotationSpeed(float days){
    return (360-days)*0.01;
}


int display[2]={1920, 1080};


void init ( void ){
    // openGL va ascunde suprafetele ce vor fi specificate
    glEnable(GL_CULL_FACE);
    // specificam sa se ascunda pe cele din spate
    glCullFace(GL_BACK);
    // precizam ordinea in care sunt desenate (invers acelor de ceasornic e default) --
    glFrontFace(GL_CW);
    // precizam ca aspura carei matrici vom stabili parametrii (in acest caz asupra matricii de proiectie)
    glMatrixMode( GL_PROJECTION );
    // stabilim perspectiva
    //gluPerspective(50, (display[0]/display[1]), 0.0, 100.0);
    gluPerspective(75, 2, 0.0, 100.0);
    // culoarea fundalului
    glClearColor (0.0 ,0.0 ,0.0 ,0.0) ;
    // mutam camera in spate cu 10 unitati ca sa vedem mai bine cubul
    glTranslatef(0, 0, -10);
    glRotatef(60, 0, 2, 1);
    glEnable(GL_TEXTURE_2D);
}

 int main ( void )
 {
    GLFWwindow * window ;
    /* Initializam libraria */
    if (! glfwInit ())
        return -1;

    /* Cream o fereastra si ii atasam un context OpenGL */
    window = glfwCreateWindow (display[0], display[1] , "My first solar system!", NULL , NULL );
    if (! window )
    {
        glfwTerminate ();
        return -1;
    }

    /* Facem fereastra curenta contextul curent */
    glfwMakeContextCurrent ( window );

    /* se initializeaza conditiile initiale, projection mode, etc. */
    init();

    GLuint sunTexture = loadTexture("sun.jpg");;
    GLuint earthTexture = loadTexture("sun.jpg");
    GLuint moonTexture = loadTexture("sun.jpg");

    float sunAngle = 0.0f;
    float earthAngle = 0.0f;
    float moonAngle = 0.0f;

    /* Loop pana cand se inchide fereastra */
    while (! glfwWindowShouldClose ( window ))
    {

        /* Aici se desenează */
        glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);

        //glPolygonMode(GL_FRONT_AND_BACK,GL_LINE); // comentati linia daca vreti sa desenati cubul plin

        glMatrixMode(GL_MODELVIEW);

        drawSphere(1.5, 50, 50, sunAngle, 0.0f, 0.0, 0.0f, sunTexture);;
        drawSphere(0.5, 50, 50, earthAngle, 5.0f, 0.0f, 5.5f, earthTexture);
        drawSphere(0.2, 50, 50, moonAngle, 5.5f, 0.0f, 5.5f, moonTexture);


        glRotatef(0.2, 0, 1, 0.5);

        sunAngle += rotationSpeed(27.0f);
        earthAngle += rotationSpeed(1.0f);
        moonAngle += rotationSpeed(27.3f);

        /* Se inverseaza bufferele */
        glfwSwapBuffers ( window );

        /* intarziem putin ca sa putem sa vedem rotatia */
        usleep(10000);

        /* Procesam evenimentelele */
        glfwPollEvents ();
    }

    glfwTerminate ();
    return 0;
 }

