#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define WIDTH 80
#define HEIGHT 25
#define MAX_OBJECTS 100

//-----------------------------------
// Structure for storing objects
//-----------------------------------

typedef struct
{
    int id;
    int type;

    int x1, y1;
    int x2, y2;

    int radius;

} Object;

Object objects[MAX_OBJECTS];
int objectCount = 0;

char canvas[HEIGHT][WIDTH];

// Shape Types

#define RECTANGLE 1
#define LINE 2
#define TRIANGLE 3
#define CIRCLE 4

//-----------------------------------
// Initialize Canvas
//-----------------------------------

void clearCanvas()
{
    int i, j;

    for(i = 0; i < HEIGHT; i++)
    {
        for(j = 0; j < WIDTH; j++)
        {
            canvas[i][j] = '_';
        }
    }
}

//-----------------------------------
// Display Canvas
//-----------------------------------

void displayCanvas()
{
    int i, j;

    printf("\n");

    for(i = 0; i < HEIGHT; i++)
    {
        for(j = 0; j < WIDTH; j++)
        {
            printf("%c", canvas[i][j]);
        }
        printf("\n");
    }
}

//-----------------------------------
// Plot Pixel
//-----------------------------------

void plot(int x, int y)
{
    if(x >= 0 && x < WIDTH && y >= 0 && y < HEIGHT)
    {
        canvas[y][x] = '*';
    }
}

//-----------------------------------
// Draw Line (Bresenham)
//-----------------------------------

void drawLineShape(int x1, int y1, int x2, int y2)
{
    int dx = abs(x2 - x1);
    int dy = abs(y2 - y1);

    int sx = (x1 < x2) ? 1 : -1;
    int sy = (y1 < y2) ? 1 : -1;

    int err = dx - dy;

    while(1)
    {
        plot(x1, y1);

        if(x1 == x2 && y1 == y2)
            break;

        int e2 = 2 * err;

        if(e2 > -dy)
        {
            err -= dy;
            x1 += sx;
        }

        if(e2 < dx)
        {
            err += dx;
            y1 += sy;
        }
    }
}

//-----------------------------------
// Draw Rectangle
//-----------------------------------

void drawRectangleShape(int x1, int y1, int x2, int y2)
{
    int x;

    for(x = x1; x <= x2; x++)
    {
        plot(x, y1);
        plot(x, y2);
    }

    int y;

    for(y = y1; y <= y2; y++)
    {
        plot(x1, y);
        plot(x2, y);
    }
}

//-----------------------------------
// Draw Triangle
//-----------------------------------

void drawTriangleShape(int x1, int y1, int x2, int y2)
{
    int x3 = (x1 + x2) / 2;
    int y3 = y1 - abs(x2 - x1) / 2;

    drawLineShape(x1, y1, x2, y2);
    drawLineShape(x1, y1, x3, y3);
    drawLineShape(x2, y2, x3, y3);
}

//-----------------------------------
// Draw Circle
//-----------------------------------

void drawCircleShape(int xc, int yc, int r)
{
    int x, y;

    for(y = yc - r; y <= yc + r; y++)
    {
        for(x = xc - r; x <= xc + r; x++)
        {
            int dx = x - xc;
            int dy = y - yc;

            double dist = sqrt(dx*dx + dy*dy);

            if(dist >= r - 0.5 && dist <= r + 0.5)
            {
                plot(x, y);
            }
        }
    }
}

//-----------------------------------
// Redraw All Objects
//-----------------------------------

void redrawCanvas()
{
    clearCanvas();

    int i;

    for(i = 0; i < objectCount; i++)
    {
        Object obj = objects[i];

        switch(obj.type)
        {
            case RECTANGLE:
                drawRectangleShape(
                    obj.x1,
                    obj.y1,
                    obj.x2,
                    obj.y2
                );
                break;

            case LINE:
                drawLineShape(
                    obj.x1,
                    obj.y1,
                    obj.x2,
                    obj.y2
                );
                break;

            case TRIANGLE:
                drawTriangleShape(
                    obj.x1,
                    obj.y1,
                    obj.x2,
                    obj.y2
                );
                break;

            case CIRCLE:
                drawCircleShape(
                    obj.x1,
                    obj.y1,
                    obj.radius
                );
                break;
        }
    }
}

//-----------------------------------
// Add Object
//-----------------------------------

void addObject()
{
    Object obj;

    obj.id = objectCount + 1;

    printf("\n1 Rectangle");
    printf("\n2 Line");
    printf("\n3 Triangle");
    printf("\n4 Circle");

    printf("\nEnter shape type: ");
    scanf("%d", &obj.type);

    if(obj.type == RECTANGLE ||
       obj.type == LINE ||
       obj.type == TRIANGLE)
    {
        printf("Enter x1 y1 : ");
        scanf("%d %d", &obj.x1, &obj.y1);

        printf("Enter x2 y2 : ");
        scanf("%d %d", &obj.x2, &obj.y2);
    }

    else if(obj.type == CIRCLE)
    {
        printf("Enter center x y : ");
        scanf("%d %d", &obj.x1, &obj.y1);

        printf("Enter radius : ");
        scanf("%d", &obj.radius);
    }

    objects[objectCount++] = obj;

    redrawCanvas();
}

//-----------------------------------
// Delete Object
//-----------------------------------

void deleteObject()
{
    int id;

    printf("Enter Object ID to delete: ");
    scanf("%d", &id);

    int i;

    for(i = 0; i < objectCount; i++)
    {
        if(objects[i].id == id)
        {
            int j;

            for(j = i; j < objectCount - 1; j++)
            {
                objects[j] = objects[j + 1];
            }

            objectCount--;
            break;
        }
    }

    redrawCanvas();
}

//-----------------------------------
// Modify Object
//-----------------------------------

void modifyObject()
{
    int id;

    printf("Enter Object ID: ");
    scanf("%d", &id);

    int i;

    for(i = 0; i < objectCount; i++)
    {
        if(objects[i].id == id)
        {
            if(objects[i].type == CIRCLE)
            {
                printf("New center x y: ");
                scanf("%d %d",
                      &objects[i].x1,
                      &objects[i].y1);

                printf("New radius: ");
                scanf("%d",
                      &objects[i].radius);
            }

            else
            {
                printf("New x1 y1: ");
                scanf("%d %d",
                      &objects[i].x1,
                      &objects[i].y1);

                printf("New x2 y2: ");
                scanf("%d %d",
                      &objects[i].x2,
                      &objects[i].y2);
            }

            break;
        }
    }

    redrawCanvas();
}

//-----------------------------------
// List Objects
//-----------------------------------

void listObjects()
{
    int i;

    printf("\nStored Objects\n");

    for(i = 0; i < objectCount; i++)
    {
        printf("ID = %d   Type = %d\n",
               objects[i].id,
               objects[i].type);
    }
}

//-----------------------------------
// Main
//-----------------------------------

int main()
{
    int choice;

    clearCanvas();

    while(1)
    {
        printf("\n");
        printf("\n===== 2D GRAPHICS EDITOR =====");
        printf("\n1. Add Object");
        printf("\n2. Delete Object");
        printf("\n3. Modify Object");
        printf("\n4. Display Picture");
        printf("\n5. List Objects");
        printf("\n6. Exit");

        printf("\nEnter Choice: ");
        scanf("%d", &choice);

        switch(choice)
        {
            case 1:
                addObject();
                break;

            case 2:
                deleteObject();
                break;

            case 3:
                modifyObject();
                break;

            case 4:
                displayCanvas();
                break;

            case 5:
                listObjects();
                break;

            case 6:
                return 0;

            default:
                printf("Invalid Choice\n");
        }
    }

    return 0;
}