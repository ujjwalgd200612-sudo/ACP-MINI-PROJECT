#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define WIDTH 80
#define HEIGHT 25
#define MAX_OBJECTS 100

#define RECTANGLE 1
#define LINE 2
#define TRIANGLE 3
#define CIRCLE 4

typedef struct
{
    int id;
    int type;

    int x1, y1;
    int x2, y2;
    int x3, y3; // Triangle third point

    int radius; // Circle radius

} Object;

Object objects[MAX_OBJECTS];

int objectCount = 0;
int nextID = 1;

char canvas[HEIGHT][WIDTH];

/* ---------------- CANVAS FUNCTIONS ---------------- */

void clearCanvas()
{
    for (int i = 0; i < HEIGHT; i++)
    {
        for (int j = 0; j < WIDTH; j++)
        {
            canvas[i][j] = '_';
        }
    }
}

void plot(int x, int y)
{
    if (x >= 0 && x < WIDTH && y >= 0 && y < HEIGHT)
    {
        canvas[y][x] = '*';
    }
}

void displayCanvas()
{
    printf("\n");

    for (int i = 0; i < HEIGHT; i++)
    {
        for (int j = 0; j < WIDTH; j++)
        {
            printf("%c", canvas[i][j]);
        }
        printf("\n");
    }
}

/* ---------------- DRAWING FUNCTIONS ---------------- */

void drawLine(int x1, int y1, int x2, int y2)
{
    int dx = abs(x2 - x1);
    int dy = abs(y2 - y1);

    int sx = (x1 < x2) ? 1 : -1;
    int sy = (y1 < y2) ? 1 : -1;

    int err = dx - dy;

    while (1)
    {
        plot(x1, y1);

        if (x1 == x2 && y1 == y2)
            break;

        int e2 = 2 * err;

        if (e2 > -dy)
        {
            err -= dy;
            x1 += sx;
        }

        if (e2 < dx)
        {
            err += dx;
            y1 += sy;
        }
    }
}

void drawRectangle(int x1, int y1, int x2, int y2)
{
    if (x1 > x2)
    {
        int t = x1;
        x1 = x2;
        x2 = t;
    }

    if (y1 > y2)
    {
        int t = y1;
        y1 = y2;
        y2 = t;
    }

    for (int x = x1; x <= x2; x++)
    {
        plot(x, y1);
        plot(x, y2);
    }

    for (int y = y1; y <= y2; y++)
    {
        plot(x1, y);
        plot(x2, y);
    }
}

void drawTriangle(int x1, int y1,
                  int x2, int y2,
                  int x3, int y3)
{
    drawLine(x1, y1, x2, y2);
    drawLine(x2, y2, x3, y3);
    drawLine(x3, y3, x1, y1);
}

void drawCircle(int xc, int yc, int r)
{
    for (int y = yc - r; y <= yc + r; y++)
    {
        for (int x = xc - r; x <= xc + r; x++)
        {
            int dx = x - xc;
            int dy = y - yc;

            double distance = sqrt(dx * dx + dy * dy);

            if (distance >= r - 0.5 && distance <= r + 0.5)
            {
                plot(x, y);
            }
        }
    }
}

/* ---------------- HELPER FUNCTIONS ---------------- */

int validCoordinate(int x, int y)
{
    return (x >= 0 && x < WIDTH &&
            y >= 0 && y < HEIGHT);
}

const char *shapeName(int type)
{
    switch (type)
    {
    case RECTANGLE:
        return "Rectangle";

    case LINE:
        return "Line";

    case TRIANGLE:
        return "Triangle";

    case CIRCLE:
        return "Circle";

    default:
        return "Unknown";
    }
}

/* ---------------- REDRAW ---------------- */

void redrawCanvas()
{
    clearCanvas();

    for (int i = 0; i < objectCount; i++)
    {
        switch (objects[i].type)
        {
        case RECTANGLE:
            drawRectangle(
                objects[i].x1,
                objects[i].y1,
                objects[i].x2,
                objects[i].y2);
            break;

        case LINE:
            drawLine(
                objects[i].x1,
                objects[i].y1,
                objects[i].x2,
                objects[i].y2);
            break;

        case TRIANGLE:
            drawTriangle(
                objects[i].x1,
                objects[i].y1,
                objects[i].x2,
                objects[i].y2,
                objects[i].x3,
                objects[i].y3);
            break;

        case CIRCLE:
            drawCircle(
                objects[i].x1,
                objects[i].y1,
                objects[i].radius);
            break;
        }
    }
}

/* ---------------- LIST OBJECTS ---------------- */

void listObjects()
{
    if (objectCount == 0)
    {
        printf("\nNo objects stored.\n");
        return;
    }

    printf("\n===== OBJECT LIST =====\n");

    for (int i = 0; i < objectCount; i++)
    {
        Object obj = objects[i];

        if (obj.type == CIRCLE)
        {
            printf("ID:%d | Circle | Center(%d,%d) Radius=%d\n",
                   obj.id,
                   obj.x1,
                   obj.y1,
                   obj.radius);
        }
        else if (obj.type == TRIANGLE)
        {
            printf("ID:%d | Triangle | (%d,%d) (%d,%d) (%d,%d)\n",
                   obj.id,
                   obj.x1, obj.y1,
                   obj.x2, obj.y2,
                   obj.x3, obj.y3);
        }
        else
        {
            printf("ID:%d | %s | (%d,%d) (%d,%d)\n",
                   obj.id,
                   shapeName(obj.type),
                   obj.x1, obj.y1,
                   obj.x2, obj.y2);
        }
    }
}

/* ---------------- ADD OBJECT ---------------- */

void addObject()
{
    if (objectCount >= MAX_OBJECTS)
    {
        printf("Maximum object limit reached.\n");
        return;
    }

    Object obj;

    obj.id = nextID++;

    printf("\n1. Rectangle\n");
    printf("2. Line\n");
    printf("3. Triangle\n");
    printf("4. Circle\n");

    printf("Enter shape type: ");
    scanf("%d", &obj.type);

    switch (obj.type)
    {
    case RECTANGLE:
    case LINE:

        printf("Enter x1 y1: ");
        scanf("%d %d", &obj.x1, &obj.y1);

        printf("Enter x2 y2: ");
        scanf("%d %d", &obj.x2, &obj.y2);

        break;

    case TRIANGLE:

        printf("Enter x1 y1: ");
        scanf("%d %d", &obj.x1, &obj.y1);

        printf("Enter x2 y2: ");
        scanf("%d %d", &obj.x2, &obj.y2);

        printf("Enter x3 y3: ");
        scanf("%d %d", &obj.x3, &obj.y3);

        break;

    case CIRCLE:

        printf("Enter center x y: ");
        scanf("%d %d", &obj.x1, &obj.y1);

        printf("Enter radius: ");
        scanf("%d", &obj.radius);

        break;

    default:
        printf("Invalid shape type.\n");
        return;
    }

    objects[objectCount++] = obj;

    redrawCanvas();

    printf("Object added successfully.\n");
}

/* ---------------- DELETE OBJECT ---------------- */

void deleteObject()
{
    int id;

    printf("Enter Object ID: ");
    scanf("%d", &id);

    for (int i = 0; i < objectCount; i++)
    {
        if (objects[i].id == id)
        {
            for (int j = i; j < objectCount - 1; j++)
            {
                objects[j] = objects[j + 1];
            }

            objectCount--;

            redrawCanvas();

            printf("Object deleted.\n");
            return;
        }
    }

    printf("Object not found.\n");
}

/* ---------------- MODIFY OBJECT ---------------- */

void modifyObject()
{
    int id;

    printf("Enter Object ID: ");
    scanf("%d", &id);

    for (int i = 0; i < objectCount; i++)
    {
        if (objects[i].id == id)
        {
            switch (objects[i].type)
            {
            case RECTANGLE:
            case LINE:

                printf("Enter new x1 y1: ");
                scanf("%d %d",
                      &objects[i].x1,
                      &objects[i].y1);

                printf("Enter new x2 y2: ");
                scanf("%d %d",
                      &objects[i].x2,
                      &objects[i].y2);

                break;

            case TRIANGLE:

                printf("Enter new x1 y1: ");
                scanf("%d %d",
                      &objects[i].x1,
                      &objects[i].y1);

                printf("Enter new x2 y2: ");
                scanf("%d %d",
                      &objects[i].x2,
                      &objects[i].y2);

                printf("Enter new x3 y3: ");
                scanf("%d %d",
                      &objects[i].x3,
                      &objects[i].y3);

                break;

            case CIRCLE:

                printf("Enter new center x y: ");
                scanf("%d %d",
                      &objects[i].x1,
                      &objects[i].y1);

                printf("Enter new radius: ");
                scanf("%d",
                      &objects[i].radius);

                break;
            }

            redrawCanvas();

            printf("Object modified.\n");
            return;
        }
    }

    printf("Object not found.\n");
}

/* ---------------- UNDO ---------------- */

void undoLastObject()
{
    if (objectCount == 0)
    {
        printf("Nothing to undo.\n");
        return;
    }

    objectCount--;

    redrawCanvas();

    printf("Last object removed.\n");
}

/* ---------------- CLEAR ---------------- */

void clearPicture()
{
    objectCount = 0;
    clearCanvas();

    printf("Picture cleared.\n");
}

/* ---------------- SAVE OBJECTS ---------------- */

void saveObjects()
{
    FILE *fp = fopen("objects.txt", "w");

    if (fp == NULL)
    {
        printf("Cannot create file.\n");
        return;
    }

    fprintf(fp, "%d\n", objectCount);

    for (int i = 0; i < objectCount; i++)
    {
        Object obj = objects[i];

        fprintf(fp,
                "%d %d %d %d %d %d %d %d %d\n",
                obj.id,
                obj.type,
                obj.x1,
                obj.y1,
                obj.x2,
                obj.y2,
                obj.x3,
                obj.y3,
                obj.radius);
    }

    fclose(fp);

    printf("Objects saved to objects.txt\n");
}

/* ---------------- LOAD OBJECTS ---------------- */

void loadObjects()
{
    FILE *fp = fopen("objects.txt", "r");

    if (fp == NULL)
    {
        printf("objects.txt not found.\n");
        return;
    }

    fscanf(fp, "%d", &objectCount);

    int maxID = 0;

    for (int i = 0; i < objectCount; i++)
    {
        fscanf(fp,
               "%d %d %d %d %d %d %d %d %d",
               &objects[i].id,
               &objects[i].type,
               &objects[i].x1,
               &objects[i].y1,
               &objects[i].x2,
               &objects[i].y2,
               &objects[i].x3,
               &objects[i].y3,
               &objects[i].radius);

        if (objects[i].id > maxID)
            maxID = objects[i].id;
    }

    nextID = maxID + 1;

    fclose(fp);

    redrawCanvas();

    printf("Objects loaded successfully.\n");
}

/* ---------------- MAIN ---------------- */

int main()
{
    int choice;

    clearCanvas();

    while (1)
    {
        printf("\n=================================\n");
        printf("      2D GRAPHICS EDITOR\n");
        printf("=================================\n");
        printf("Objects Stored : %d\n\n", objectCount);

        printf("1. Add Object\n");
        printf("2. Delete Object\n");
        printf("3. Modify Object\n");
        printf("4. Display Picture\n");
        printf("5. List Objects\n");
        printf("6. Undo Last Object\n");
        printf("7. Clear Picture\n");
        printf("8. Save Objects\n");
        printf("9. Load Objects\n");
        printf("10. Exit\n");

        printf("Enter choice: ");
        scanf("%d", &choice);

        switch (choice)
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
            undoLastObject();
            break;

        case 7:
            clearPicture();
            break;

        case 8:
            saveObjects();
            break;

        case 9:
            loadObjects();
            break;

        case 10:
            printf("Exiting...\n");
            return 0;

        default:
            printf("Invalid choice.\n");
        }
    }

    return 0;
}