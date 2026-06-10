#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define WIDTH 80
#define HEIGHT 25
#define MAX_OBJECTS 50

#define RECTANGLE 1
#define LINE 2
#define TRIANGLE 3
#define CIRCLE 4

typedef struct {
    int id;
    int type;
    int x1, y1;
    int x2, y2;
    int radius;
} Object;

Object objects[MAX_OBJECTS];
int objectCount = 0;
int nextID = 1;

char canvas[HEIGHT][WIDTH];

void clearCanvas() {
    for (int i = 0; i < HEIGHT; i++)
        for (int j = 0; j < WIDTH; j++)
            canvas[i][j] = '_';
}

void plot(int x, int y) {
    if (x >= 0 && x < WIDTH && y >= 0 && y < HEIGHT)
        canvas[y][x] = '*';
}

void displayCanvas() {
    printf("\n");
    for (int i = 0; i < HEIGHT; i++) {
        for (int j = 0; j < WIDTH; j++)
            printf("%c", canvas[i][j]);
        printf("\n");
    }
}

void drawLine(int x1, int y1, int x2, int y2) {
    int dx = abs(x2 - x1), dy = abs(y2 - y1);
    int sx = (x1 < x2) ? 1 : -1;
    int sy = (y1 < y2) ? 1 : -1;
    int err = dx - dy;

    while (1) {
        plot(x1, y1);

        if (x1 == x2 && y1 == y2)
            break;

        int e2 = 2 * err;

        if (e2 > -dy) {
            err -= dy;
            x1 += sx;
        }

        if (e2 < dx) {
            err += dx;
            y1 += sy;
        }
    }
}

void drawRectangle(int x1, int y1, int x2, int y2) {
    if (x1 > x2) { int t = x1; x1 = x2; x2 = t; }
    if (y1 > y2) { int t = y1; y1 = y2; y2 = t; }

    for (int x = x1; x <= x2; x++) {
        plot(x, y1);
        plot(x, y2);
    }

    for (int y = y1; y <= y2; y++) {
        plot(x1, y);
        plot(x2, y);
    }
}

void drawTriangle(int x1, int y1, int x2, int y2) {
    int x3 = (x1 + x2) / 2;
    int y3 = y1 - 5;
    if (y3 < 0) y3 = 0;

    drawLine(x1, y1, x2, y2);
    drawLine(x1, y1, x3, y3);
    drawLine(x2, y2, x3, y3);
}

void drawCircle(int xc, int yc, int r) {
    for (int y = yc - r; y <= yc + r; y++) {
        for (int x = xc - r; x <= xc + r; x++) {
            int dx = x - xc;
            int dy = y - yc;
            double d = sqrt(dx * dx + dy * dy);

            if (d >= r - 0.5 && d <= r + 0.5)
                plot(x, y);
        }
    }
}

void redrawCanvas() {
    clearCanvas();

    for (int i = 0; i < objectCount; i++) {
        switch (objects[i].type) {
            case RECTANGLE:
                drawRectangle(objects[i].x1, objects[i].y1,
                              objects[i].x2, objects[i].y2);
                break;
            case LINE:
                drawLine(objects[i].x1, objects[i].y1,
                         objects[i].x2, objects[i].y2);
                break;
            case TRIANGLE:
                drawTriangle(objects[i].x1, objects[i].y1,
                             objects[i].x2, objects[i].y2);
                break;
            case CIRCLE:
                drawCircle(objects[i].x1, objects[i].y1,
                           objects[i].radius);
                break;
        }
    }
}

const char *shapeName(int type) {
    switch (type) {
        case RECTANGLE: return "Rectangle";
        case LINE: return "Line";
        case TRIANGLE: return "Triangle";
        case CIRCLE: return "Circle";
        default: return "Unknown";
    }
}

void listObjects() {
    if (objectCount == 0) {
        printf("\nNo objects available.\n");
        return;
    }

    printf("\nStored Objects:\n");

    for (int i = 0; i < objectCount; i++) {
        if (objects[i].type == CIRCLE)
            printf("ID:%d Shape:%s Center(%d,%d) Radius=%d\n",
                   objects[i].id, shapeName(objects[i].type),
                   objects[i].x1, objects[i].y1, objects[i].radius);
        else
            printf("ID:%d Shape:%s (%d,%d)-(%d,%d)\n",
                   objects[i].id, shapeName(objects[i].type),
                   objects[i].x1, objects[i].y1,
                   objects[i].x2, objects[i].y2);
    }
}

void addObject() {
    if (objectCount >= MAX_OBJECTS) {
        printf("Maximum objects reached.\n");
        return;
    }

    Object obj;
    obj.id = nextID++;

    printf("\n1.Rectangle\n2.Line\n3.Triangle\n4.Circle\n");
    printf("Enter shape type: ");
    scanf("%d", &obj.type);

    if (obj.type == CIRCLE) {
        printf("Enter center x y: ");
        scanf("%d%d", &obj.x1, &obj.y1);
        printf("Enter radius: ");
        scanf("%d", &obj.radius);
    } else {
        printf("Enter x1 y1: ");
        scanf("%d%d", &obj.x1, &obj.y1);
        printf("Enter x2 y2: ");
        scanf("%d%d", &obj.x2, &obj.y2);
    }

    objects[objectCount++] = obj;
    redrawCanvas();
    printf("Object added successfully.\n");
}

void deleteObject() {
    int id;
    printf("Enter object ID: ");
    scanf("%d", &id);

    for (int i = 0; i < objectCount; i++) {
        if (objects[i].id == id) {
            for (int j = i; j < objectCount - 1; j++)
                objects[j] = objects[j + 1];

            objectCount--;
            redrawCanvas();
            printf("Object deleted.\n");
            return;
        }
    }
    printf("Object not found.\n");
}

void modifyObject() {
    int id;
    printf("Enter object ID: ");
    scanf("%d", &id);

    for (int i = 0; i < objectCount; i++) {
        if (objects[i].id == id) {
            if (objects[i].type == CIRCLE) {
                printf("Enter new center x y: ");
                scanf("%d%d", &objects[i].x1, &objects[i].y1);
                printf("Enter new radius: ");
                scanf("%d", &objects[i].radius);
            } else {
                printf("Enter new x1 y1: ");
                scanf("%d%d", &objects[i].x1, &objects[i].y1);
                printf("Enter new x2 y2: ");
                scanf("%d%d", &objects[i].x2, &objects[i].y2);
            }

            redrawCanvas();
            printf("Object modified.\n");
            return;
        }
    }
    printf("Object not found.\n");
}

void clearPicture() {
    objectCount = 0;
    clearCanvas();
    printf("Picture cleared.\n");
}

void savePicture() {
    FILE *fp = fopen("picture.txt", "w");
    if (!fp) return;

    for (int i = 0; i < HEIGHT; i++) {
        for (int j = 0; j < WIDTH; j++)
            fputc(canvas[i][j], fp);
        fputc('\n', fp);
    }

    fclose(fp);
    printf("Picture saved to picture.txt\n");
}

int main() {
    int choice;
    clearCanvas();

    while (1) {
        printf("\n===== 2D GRAPHICS EDITOR =====\n");
        printf("1. Add Object\n");
        printf("2. Delete Object\n");
        printf("3. Modify Object\n");
        printf("4. Display Picture\n");
        printf("5. List Objects\n");
        printf("6. Clear Picture\n");
        printf("7. Save Picture\n");
        printf("8. Exit\n");
        printf("Enter choice: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1: addObject(); break;
            case 2: deleteObject(); break;
            case 3: modifyObject(); break;
            case 4: displayCanvas(); break;
            case 5: listObjects(); break;
            case 6: clearPicture(); break;
            case 7: savePicture(); break;
            case 8: return 0;
            default: printf("Invalid choice.\n");
        }
    }
}
