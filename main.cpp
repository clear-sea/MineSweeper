#include<graphics.h>
#include<iostream>
#include<thread>
#include<ctime>

using namespace std;
//全局变量
MOUSEMSG msg;
IMAGE mine, grid, grid_motion, grid_down, flag, flag_motion;
const int width = 400, height = 400, grid_size = 40,mines_num_copy=10;
int all_num=0,grid_x = 0, grid_y = 0, mines_num = 10, layout_size = width / grid_size;

//格子类，边长为40像素的正方形
class Grid {
public:
	bool is_mine, is_click = false, is_flag = false;
	int num = 0;
	IMAGE mine;
	int x1, x2, y1, y2;//格子的四个角顶点的位置
	int i, j;
	int grids[8][2] = { 0 };

	void init(bool is_mine, int x, int y, int i, int j) {
		//初始化
		this->is_mine = is_mine;
		x1 = x; x2 = x + grid_size;
		y1 = y; y2 = y + grid_size;
		this->i = i; this->j = j;
		//绘制
		change(grid);
		//判断自己的位置(grids二维数组记录周围8个格子的行和列)
		if (i == 0 && j == 0)
		{
			//左上角
			int grids[8][2] = { {0,1},{2,2},{1,0},{1,1},{1,2},{2,0},{2,1},{2,2} };
			for (int a = 0; a < 8; a++) {
				this->grids[a][0] = grids[a][0];
				this->grids[a][1] = grids[a][1];
			}
		}

		else if (i == layout_size - 1 && j == layout_size - 1)
		{
			//右下角
			int grids[8][2] = { {layout_size - 1,layout_size - 2},{layout_size - 1,layout_size - 3},{layout_size - 2,layout_size - 1},{layout_size - 2,layout_size - 2},{layout_size - 2,layout_size - 3},{layout_size - 3,layout_size - 1},{layout_size - 3,layout_size - 2},{layout_size - 3,layout_size - 3} };
			for (int a = 0; a < 8; a++) {
				this->grids[a][0] = grids[a][0];
				this->grids[a][1] = grids[a][1];
			}
		}

		else if (i == 0 && j == layout_size - 1)
		{
			//右上角
			int grids[8][2] = { {0,layout_size - 2},{0,layout_size - 3},{1,layout_size - 1},{1,layout_size - 2},{1,layout_size - 3},{2,layout_size - 1},{2,layout_size - 2},{2,layout_size - 3} };
			for (int a = 0; a < 8; a++) {
				this->grids[a][0] = grids[a][0];
				this->grids[a][1] = grids[a][1];
			}
		}

		else if (i == layout_size - 1 && j == 0){
			//左下角
			int grids[8][2] = { {layout_size - 3,0},{layout_size - 3,1},{layout_size - 3,2},{layout_size - 2,0},{layout_size - 2,1},{layout_size - 2,2},{layout_size - 1,1},{layout_size - 1,2} };
			for (int a = 0; a < 8; a++) {
				this->grids[a][0] = grids[a][0];
				this->grids[a][1] = grids[a][1];
			}
		}
			
		else if (i == 0 && j > 0 && j < layout_size - 1) 
		{
			//上-边
			int grids[8][2] = { {0,j - 1},{0,j + 1},{1,j - 1},{1,j},{1,j + 1},{2,j - 1},{2,j},{2,j + 1} };
			for (int a = 0; a < 8; a++) {
				this->grids[a][0] = grids[a][0];
				this->grids[a][1] = grids[a][1];
			}
		}

		else if (i == layout_size - 1 && j > 0 && j < layout_size - 1) 
		{
			//下-边
			int grids[8][2] = { {layout_size - 1,j - 1},{layout_size - 1,j + 1} ,{layout_size - 2,j - 1} ,{layout_size - 2,j} ,{layout_size - 2,j + 1} ,{layout_size - 3,j - 1} ,{layout_size - 3,j} ,{layout_size - 3,j + 1} };
			for (int a = 0; a < 8; a++) {
				this->grids[a][0] = grids[a][0];
				this->grids[a][1] = grids[a][1];
			}
		}
			
		else if (j == 0 && i > 0 && i < layout_size - 1) 
		{
			//左-边
			int grids[8][2] = { {i - 1,0} ,{i - 1,1} ,{i - 1,2} ,{i,1} ,{i,2} ,{i + 1,0} ,{i + 1,1} ,{i + 1,2} };
			for (int a = 0; a < 8; a++) {
				this->grids[a][0] = grids[a][0];
				this->grids[a][1] = grids[a][1];
			}
		}

		else if (j == layout_size - 1 && i > 0 && i < layout_size - 1) 
		{
			//右-边
			int grids[8][2] = { {i - 1,layout_size - 1} ,{i - 1,layout_size - 2} ,{i - 1,layout_size - 3} ,{i,layout_size - 2} ,{i,layout_size - 3} ,{i + 1,layout_size - 1} ,{i + 1,layout_size - 2} ,{i + 1,layout_size - 3} };
			for (int a = 0; a < 8; a++) {
				this->grids[a][0] = grids[a][0];
				this->grids[a][1] = grids[a][1];
			}
		}

		else
		{
			//以此格子为中心
			int grids[8][2] = { {i - 1,j - 1} ,{i - 1,j} ,{i - 1,j + 1} ,{i,j - 1} ,{i,j + 1} ,{i + 1,j - 1} ,{i + 1,j} ,{i + 1,j + 1} };
			for (int a = 0; a < 8; a++) {
				this->grids[a][0] = grids[a][0];
				this->grids[a][1] = grids[a][1];
			}
		}
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

	void find_0(Grid(&layout)[10][10]) {
		for (int a = 0; a < 8; a++) {//遍历grids（周围格子）
			int i = grids[a][0], j = grids[a][1];
			if (layout[i][j].num == 0 && layout[i][j].is_click == false && layout[i][j].is_mine == false) {
				//模拟点击操作
				layout[i][j].is_flag = false;
				layout[i][j].change(grid_down);
				layout[i][j].is_click = true;
				all_num++;
				layout[i][j].find_0(layout);
			}
		}
	}
};


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
	Grid layout[width / grid_size][height / grid_size];//布局，用二维数组，如果是1就有雷，0则没有

	srand(time(nullptr));
	for (int i = 0; i < layout_size; i++) {
		for (int j = 0; j < layout_size; j++) {
			is_mine = rand() % 2;
			
			if (is_mine && mines_num > 0 && i!=0 && j!=0) {
				layout[i][j].init(true, grid_x, grid_y, i, j);
				mines_num--;
			}
			else if (is_mine && mines_num <= 0)
				layout[i][j].init(false, grid_x, grid_y, i, j);
			else
				layout[i][j].init(false, grid_x, grid_y, i, j);

			grid_x += 40;
		}
		grid_y += 40;
		grid_x = 0;
	}

	for (int i = 0; i < layout_size; i++) {
		for (int j = 0; j < layout_size; j++) {
			//每个格子获取周围格子的雷数
			for (int a = 0; a < 8; a++) {
				if (layout[layout[i][j].grids[a][0]][layout[i][j].grids[a][1]].is_mine) {
					layout[i][j].num++;
				}
			}
		}
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
								//显示格子周围的雷数
								layout[i][j].show_num();
								//如果是数量是0
								if (layout[i][j].num == 0)
									layout[i][j].find_0(layout);
							}
							layout[i][j].is_click = true;
							all_num++;
						}
						//判断未点开的格子数，从而判断输赢
						if (all_num == layout_size * layout_size - mines_num_copy) {
							//显示提示信息
							setbkmode(TRANSPARENT);
							settextcolor(GREEN);
							settextstyle(50, 0, _T("Consolas"));
							outtextxy(width / 2 - 180, height / 2 - 30, _T("获胜！即将退出"));
							Sleep(2500); //停顿两秒半
							//退出
							return 0;
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