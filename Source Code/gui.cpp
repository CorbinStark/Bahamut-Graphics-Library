//#include "gui.h"
//
//void* hot_item;
//void* active_item;
//
//vec2f followpoint;
//
//bool buttonPressed(Button& button, int x, int y) {
//	bool result = false;
//	Rectangle buttonRect;
//	buttonRect.x = x;
//	buttonRect.y = y;
//	buttonRect.width = button.cold.width + 5;
//	buttonRect.height = button.cold.height + 5;
//	bool inside = colliding(buttonRect, getMousePos());
//
//	if (inside && active_item != &button)
//		hot_item = &button;
//	else
//		hot_item = 0;
//
//	if (hot_item == &button && isButtonPressed(GLFW_MOUSE_BUTTON_LEFT)) {
//		active_item = &button;
//	}
//
//	if (active_item == &button && isButtonReleased(GLFW_MOUSE_BUTTON_LEFT)) {
//		active_item = 0;
//		hot_item = 0;
//		if(inside)
//			result = true;
//	}
//
//	if (hot_item == &button || active_item == &button) {
//		drawTexture(button.hot, x, y);
//	}
//	else {
//		drawTexture(button.cold, x, y);
//	}
//	drawTexture(button.text, x + (buttonRect.width / 2) - (button.text.width / 2), y + (buttonRect.height / 2) - (button.text.height / 2));
//
//	return result;
//}

//int menuPressed(Menu& menu) {
//	int result = -1;
//
//	vec2f mouse_pos = getMousePos();
//
//	int w = 0;
//	int h = 0;
//	for (int i = 0; i < menu.buttons.size(); ++i) {
//		w = std::fmax(w, menu.buttons[i].cold.width);
//		h = std::fmax(h, menu.buttons[i].cold.height);
//	}
//	w += 20;
//	h *= menu.buttons.size();
//	h += 45;
//
//	drawRectangle(menu.x, menu.y , w, h, vec4f(228, 213, 175, 255));
//	drawRectangle(menu.x, menu.y, w, 2, BLACK);
//	drawRectangle(menu.x, menu.y + 20, w, 2, BLACK);
//	drawRectangle(menu.x, menu.y, 2, h, BLACK);
//	drawRectangle(menu.x + w, menu.y, 2, h, BLACK);
//	drawRectangle(menu.x, menu.y + h, w, 2, BLACK);
//
//	//menu.corner.flip_flag = 0;
//	//drawTexture(menu.corner, menu.x, menu.y);
//	//menu.corner.flip_flag = FLIP_HORIZONTAL;
//	//drawTexture(menu.corner, menu.x + w - menu.corner.width, menu.y);
//	//menu.corner.flip_flag = FLIP_VERTICAL;
//	//drawTexture(menu.corner, menu.x, menu.y + h - menu.corner.height);
//	//menu.corner.flip_flag = FLIP_HORIZONTAL | FLIP_VERTICAL;
//	//drawTexture(menu.corner, menu.x + w - menu.corner.width, menu.y + h - menu.corner.height);
//
//	int yPos = menu.y + 25;
//	int xPos = menu.x + (w / 2);
//	for (int i = 0; i < menu.buttons.size(); ++i) {
//		if (buttonPressed(menu.buttons[i], xPos - (menu.buttons[i].cold.width / 2), yPos))
//			result = i;
//
//		yPos += menu.buttons[i].cold.height + 5;
//	}
//
//	bool inside = colliding(Rectangle(menu.x, menu.y, w, 20), mouse_pos);
//
//	if (inside && active_item != &menu)
//		hot_item = &menu;
//	else
//		hot_item = 0;
//	
//	if (hot_item == &menu && isButtonPressed(GLFW_MOUSE_BUTTON_LEFT)) {
//		active_item = &menu;
//		followpoint = vec2f(mouse_pos.x - menu.x, mouse_pos.y - menu.y);
//	}
//
//	if (active_item == &menu && isButtonReleased(GLFW_MOUSE_BUTTON_LEFT)) {
//		active_item = 0;
//		hot_item = 0;
//		if (inside) {
//
//		}
//	}
//
//	if (active_item == &menu) {
//		menu.x = (mouse_pos.x - followpoint.x);
//		menu.y = (mouse_pos.y - followpoint.y);
//	}
//
//	return result;
//}