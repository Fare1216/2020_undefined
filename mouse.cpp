#include"DxLib.h"


/*===== マウスXY座標取得後範囲内だったら何をするか =====*/
void MouseXY(int mouse_x_point, int mouse_y_point, int left_x_point, int left_y_point, int right_x_point, int right_y_point, int *now_scene, int change_scene, int Mouse, int *cleck_flag){
	if (left_x_point <= mouse_x_point && mouse_x_point < right_x_point && left_y_point <= mouse_y_point && mouse_y_point < right_y_point){
		if (Mouse & MOUSE_INPUT_LEFT && *cleck_flag == 0){
			*now_scene = change_scene;
			*cleck_flag = 1;
		}
	}
}

/*===== マウスの終了判定 =====*/
void MouseEND(int Mouse, int *cleck_flag){
	if (Mouse & MOUSE_INPUT_LEFT){
		*cleck_flag = 1;
	}
	else if ((Mouse & MOUSE_INPUT_LEFT) == 0){
		*cleck_flag = 0;
	}
}