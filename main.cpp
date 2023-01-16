#include<graphics.h>
#include<iostream>
#include<random>
#include<ctime>
using namespace std;
//全局变量
MOUSEMSG msg;
IMAGE mine,grid,grid_motion,grid_down,flag,flag_motion;
const int width = 400, height = 400, grid_size = 40;
int grid_x = 0, grid_y = 0, mines_num = 10, layout_size = width / grid_size;

//格子类，边长为40像素的正方形
class Grid {
	public:
		bool is_mine,is_click=false,is_flag=false;
		int num=0;
		IMAGE mine;
		int x1, x2, y1, y2;//格子的四个角顶点的位置
		int a, b;

		void init(bool is_mine,int x,int y,int a,int b) {
			//初始化
			this->is_mine = is_mine;
			x1 = x; x2 = x + grid_size;
			y1 = y; y2 = y + grid_size;
			this->a = a; this->b = b;
			//绘制
			change(grid);
		}

		void change(IMAGE image) {
			putimage(x1, y1, &image);
		}

		void show_num() {
			if (num == 0)
				return;
			else if (num == 1)
				settextcolor(BLUE);
			else if (num == 2)
				settextcolor(GREEN);
			else if (num == 3)
				settextcolor(YELLOW);
			else if (num == 4)
				settextcolor(RGB(255, 135, 35));
			else
				settextcolor(RED);
			setbkmode(TRANSPARENT);
			settextstyle(45, 0, _T("Consolas"));
			char info = num + 48;
			outtextxy(x1 + 6, y1 - 3, _T(info));
		}
};
/*
void find_0(Grid grid,Grid grids[]) {
	if (grid.num == 0 && grid.is_mine == false) {//如果这个格子是0，判断周围8个格子是否是0
		for (int a = 0; a < 8; a++) {//遍历grids（周围格子）
			Grid temp = grids[a];
			if (layout[temp.a][temp.b].num == 0&& layout[temp.a][temp.b].is_click==false&& layout[temp.a][temp.b].is_mine==false) {
				//模拟点击操作
				layout[temp.a][temp.b].is_flag = false;
				layout[temp.a][temp.b].change(grid_down);
				layout[temp.a][temp.b].is_click = true;
			}
		}
	}
}
*/

int main() {
	//程序初始化
	bool is_mine;

	initgraph(width, height);
	setbkcolor(WHITE);
	cleardevice();
	//加载图像
	loadimage(&mine, "images/mine.png", grid_size, grid_size);
	loadimage(&grid, "images/grid.png", grid_size, grid_size);
	loadimage(&grid_motion, "images/grid_motion.png", grid_size, grid_size);
	loadimage(&grid_down, "images/grid_down.png", grid_size, grid_size);
	loadimage(&flag, "images/flag.png", grid_size, grid_size);
	loadimage(&flag_motion, "images/flag_motion.png", grid_size, grid_size);
	//布雷
	//布局，用二维数组，如果是1就有雷，0则没有
	Grid layout[width / grid_size][height / grid_size];

	srand(time(nullptr));
	for (int i = 0; i < layout_size; i++) {
		for (int j = 0; j < layout_size; j++) {
			is_mine = rand() % 2;

			if (is_mine && mines_num > 0) {
				layout[i][j].init(true, grid_x, grid_y, i, j);
				mines_num--;
			}
			else if (is_mine && mines_num <= 0)
				layout[i][j].init(false, grid_x, grid_y, i, j);
			else if (is_mine == false)
				layout[i][j].init(false, grid_x, grid_y, i, j);

			grid_x += 40;
		}
		grid_y += 40;
		grid_x = 0;
	}

	/*调试输出布局
	for (int i = 0; i < layout_size; i++) {
		for (int j = 0; j < layout_size; j++) {
			cout << layout[i][j].is_mine << ' ';
		}
		cout << endl;
	}*/

	//轮询监测鼠标事件
	while (true) {
		//监测鼠标操作
		ExMessage msg = getmessage(EX_MOUSE);

		if (msg.message == WM_LBUTTONDOWN) {
			for (int i = 0; i < layout_size; i++) {
				for (int j = 0; j < layout_size; j++) {
					//通过点击坐标判断是哪个格子
					if (msg.x >= layout[i][j].x1 && msg.x <= layout[i][j].x2 && msg.y >= layout[i][j].y1 && msg.y <= layout[i][j].y2) {
						if (layout[i][j].is_click == false) {//判断是否被点击过
							layout[i][j].is_flag = false;
							//如果这个格子是雷
							if (layout[i][j].is_mine) {
								layout[i][j].change(mine);
								//显示提示信息
								setbkmode(TRANSPARENT);
								settextcolor(RED);
								settextstyle(50, 0, _T("Consolas"));
								outtextxy(width / 2 - 180, height / 2 - 30, _T("踩雷了！即将退出"));
								Sleep(2500); //停顿两秒半
								//退出程序
								return 0;
							}

							else {
								layout[i][j].change(grid_down);
								//判断格子周围的雷数
								if (i == 0 && j == 0) {
									//左上角
									Grid grids[8] = { layout[0][1],layout[2][2],layout[1][0],layout[1][1],layout[1][2],layout[2][0],layout[2][1],layout[2][2] };
									for (int a = 0; a < 8; a++) {
										if (grids[a].is_mine)
											layout[i][j].num++;
									}
								}
								else if (i == layout_size - 1 && j == layout_size - 1) {
									//右下角
									Grid grids[8] = { layout[layout_size - 1][layout_size - 2],layout[layout_size - 1][layout_size - 3],layout[layout_size - 2][layout_size - 1],layout[layout_size - 2][layout_size - 2],layout[layout_size - 2][layout_size - 3],layout[layout_size - 3][layout_size - 1],layout[layout_size - 3][layout_size - 2],layout[layout_size - 3][layout_size - 3] };
									for (int a = 0; a < 8; a++) {
										if (grids[a].is_mine)
											layout[i][j].num++;
									}

								}
								else if (i == 0 && j == layout_size - 1) {
									//右上角
									Grid grids[8] = { layout[0][layout_size - 2],layout[0][layout_size - 3],layout[1][layout_size - 1],layout[1][layout_size - 2],layout[1][layout_size - 3],layout[2][layout_size - 1],layout[2][layout_size - 2],layout[2][layout_size - 3] };
									for (int a = 0; a < 8; a++) {
										if (grids[a].is_mine)
											layout[i][j].num++;
									}

								}
								else if (i == layout_size - 1 && j == 0) {
									//左下角
									Grid grids[8] = { layout[layout_size - 3][0],layout[layout_size - 3][1],layout[layout_size - 3][2],layout[layout_size - 2][0],layout[layout_size - 2][1],layout[layout_size - 2][2],layout[layout_size - 1][1],layout[layout_size - 1][2] };
									for (int a = 0; a < 8; a++) {
										if (grids[a].is_mine)
											layout[i][j].num++;
									}

								}
								else if (i == 0 && j > 0 && j < layout_size - 1) {
									//上-边
									Grid grids[8] = { layout[0][j - 1],layout[0][j + 1],layout[1][j - 1],layout[1][j],layout[1][j + 1],layout[2][j - 1],layout[2][j],layout[2][j + 1] };
									for (int a = 0; a < 8; a++) {
										if (grids[a].is_mine)
											layout[i][j].num++;
									}

								}
								else if (i == layout_size - 1 && j > 0 && j < layout_size - 1) {
									//下-边
									Grid grids[8] = { layout[layout_size - 1][j - 1],layout[layout_size - 1][j + 1],layout[layout_size - 2][j - 1],layout[layout_size - 2][j],layout[layout_size - 2][j + 1],layout[layout_size - 3][j - 1],layout[layout_size - 3][j],layout[layout_size - 3][j + 1] };
									for (int a = 0; a < 8; a++) {
										if (grids[a].is_mine)
											layout[i][j].num++;
									}

								}
								else if (j == 0 && i > 0 && i < layout_size - 1) {
									//左-边
									Grid grids[8] = { layout[i - 1][0],layout[i - 1][1],layout[i - 1][2],layout[i][1],layout[i][2],layout[i + 1][0],layout[i + 1][1],layout[i + 1][2] };
									for (int a = 0; a < 8; a++) {
										if (grids[a].is_mine)
											layout[i][j].num++;
									}

								}
								else if (j == layout_size-1 && i > 0 && i < layout_size-1) {
									//右-边
									Grid grids[8] = { layout[i - 1][layout_size - 1],layout[i - 1][layout_size - 2],layout[i - 1][layout_size - 3],layout[i][layout_size - 2],layout[i][layout_size - 3],layout[i + 1][layout_size - 1],layout[i + 1][layout_size - 2],layout[i + 1][layout_size - 3] };
									for (int a = 0; a < 8; a++) {
										if (grids[a].is_mine)
											layout[i][j].num++;
									}

									
								}
								else if (i > 0 && i < 9 && j>0 && j < 9) {
									//以此格子为中心
									Grid grids[8] = { layout[i - 1][j - 1],layout[i - 1][j],layout[i - 1][j + 1],layout[i][j - 1],layout[i][j + 1],layout[i + 1][j - 1],layout[i + 1][j],layout[i + 1][j + 1] };
									for (int a = 0; a < 8; a++) {
										if (grids[a].is_mine)
											layout[i][j].num++;
									}

								}
								layout[i][j].show_num();
							}
							layout[i][j].is_click = true;
						}

					}
				}
			}
		}
		
		if (msg.message == WM_MOUSEMOVE) {
			for (int i = 0; i < layout_size; i++) {
				for (int j = 0; j < layout_size; j++) {
					//通过坐标判断是哪个格子
					if (msg.x >= layout[i][j].x1 && msg.x <= layout[i][j].x2 && msg.y >= layout[i][j].y1 && msg.y <= layout[i][j].y2) {
						if (layout[i][j].is_click) {
							layout[i][j].change(grid_down);
							layout[i][j].show_num();
						}
						else if (layout[i][j].is_click == false && layout[i][j].is_flag == false)
							layout[i][j].change(grid_motion);
						else if (layout[i][j].is_click == false && layout[i][j].is_flag == true)
							layout[i][j].change(flag_motion);
					}

					else {
						if (layout[i][j].is_click) {
							layout[i][j].change(grid_down);
							layout[i][j].show_num();
						}
						else if (layout[i][j].is_click == false && layout[i][j].is_flag == false)
							layout[i][j].change(grid);
						else if (layout[i][j].is_click == false && layout[i][j].is_flag)
							layout[i][j].change(flag);
					}

				}
			}
		}

		else if (msg.message == WM_RBUTTONDOWN) {
			for (int i = 0; i < layout_size; i++) {
				for (int j = 0; j < layout_size; j++) {
					//通过点击坐标判断是哪个格子
					if (msg.x >= layout[i][j].x1 && msg.x <= layout[i][j].x2 && msg.y >= layout[i][j].y1 && msg.y <= layout[i][j].y2) {
						//如果已经被标记就取消标记
						if (layout[i][j].is_flag) {
							layout[i][j].is_flag = false;
							layout[i][j].change(grid);
						}
						//如果没有被标记就标记上
						else if (layout[i][j].is_flag == false && layout[i][j].is_click == false) {
							layout[i][j].is_flag = true;
							layout[i][j].change(flag);
						}
					}
				}
			}
		}
	}


	closegraph();
	return 0;
}