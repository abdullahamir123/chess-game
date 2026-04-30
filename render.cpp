#include "render.h"

const int width = 1600;
const int height = 1000;
void Render::window() {
	InitWindow(width, height, "chess game");

	while (!WindowShouldClose()) {
		BeginDrawing();
		ClearBackground(WHITE);
		Render::mainGrid();
		EndDrawing();
	}
	CloseWindow();
}

void Render::mainGrid() {
	float offsetX = 250;
	float offsetY = 0;
	float cellWidth = (width - 250 - 250)/8; //need to subtract twice for offsets on both sides
	float cellHeight = height / 8;

	for (int i = 0; i < 8; i++) {
		for (int j = 0; j < 8; j++) {
			//pre defined struct in raylib. takes args: xoffset, yoffset, width, height
			Rectangle cell = { offsetX + j * cellWidth, offsetY + i * cellHeight, cellWidth, cellHeight };

			Color color = ((i + j) % 2) ? BROWN : DARKBROWN;

			DrawRectangleRec(cell, color);
		}
	}

}