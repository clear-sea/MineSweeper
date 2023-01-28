#include<graphics.h>
#include<iostream>
#include<vector>
#include<string>

using namespace std;
//全局变量
MOUSEMSG msg;//全局鼠标信息变量
IMAGE mine, grid, grid_motion, grid_down, flag, flag_motion , mine_other;//所有图像变量
//单个格子大小(单位：像素)，雷数常量，格子布局长宽(单位：格子)，窗口宽度，窗口高度
int grid_size = 40, layout_size = 10, width = 400, height = 400;

int all_num = 0, grid_x = 0, grid_y = 0, mines_num = 10;

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
		//以此格子为中心
		int grids[8][2] = { {i - 1,j - 1} ,{i - 1,j} ,{i - 1,j + 1} ,{i,j - 1} ,{i,j + 1} ,{i + 1,j - 1} ,{i + 1,j} ,{i + 1,j + 1} };
		for (int a = 0; a < 8; a++) {
			this->grids[a][0] = grids[a][0];
			this->grids[a][1] = grids[a][1];
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

	void find_0(vector<vector<Grid>> &layout) {
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

class Button {
public:
	int x, y, width, height;
	const char* text="NULL";

	void show(const char* text, int x, int y, int width, int height, COLORREF color1 = WHITE)
	{
		this->x = x; this->y = y;
		this->width = width; this->height = height;
		this->text = text;

		setlinecolor(color1);//设置框边颜色
		setbkmode(TRANSPARENT);//设置字体背景透明
		setfillcolor(color1);//设置填充颜色
		fillroundrect(x, y, x + width, y + height, 10, 10);//画一个按钮框

		settextcolor(BLACK);
		settextstyle(40, 0, "黑体");
		int tx = x + (width - textwidth(text)) / 2;
		int ty = y + (height - textheight(text)) / 2;
		outtextxy(tx, ty, text);
	}

	void change(COLORREF color2=RGB(237, 237, 237)) {
		setlinecolor(color2);//设置框边颜色
		setfillcolor(color2);//设置填充颜色
		fillroundrect(x, y, x + width, y + height, 10, 10);//画一个按钮框

		settextcolor(BLACK);
		settextstyle(40, 0, "黑体");
		int tx = x + (width - textwidth(text)) / 2;
		int ty = y + (height - textheight(text)) / 2;
		outtextxy(tx, ty, text);
	}
};

void settings() {
	int temp=0;
	cout << "设置格子大小(单位：像素)" << endl;
	cin >> grid_size;
	cout << "设置雷盘边长(单位：格子)" << endl;
	cin >> layout_size;
	width = height = layout_size * grid_size;
	cout << width << ' ' << height << endl;
	cout << "设置雷数" << endl;
	cin >> temp;
	if (temp >= layout_size * layout_size)
		cout << "设置的雷太多了" << endl;
	else {
		mines_num = temp;
		cout << "OK 开始！" << endl;
		return;
	}
}

void start() {
	//程序初始化
	initgraph(width, height);
	setbkcolor(BLACK);
	cleardevice();
	setbkcolor(WHITE);
	//绘制设置按钮

	Button btn1;
	btn1.show("设置", 150, 100, 100, 50);
	Button btn2;
	btn2.show("开始", 150, 200, 100, 50);
	//死循环
	while (true) {
		ExMessage msg = getmessage(EX_MOUSE);

		if (msg.message == WM_LBUTTONDOWN) {
			if (msg.x >= btn1.x && msg.x <= btn1.x + btn1.width && msg.y >= btn1.y && msg.y <= btn1.y + btn1.height) {
				btn1.change();
				settings();
				cleardevice();
				initgraph(width, height);
				return;
			}

			else if (msg.x >= btn2.x && msg.x <= btn2.x + btn2.width && msg.y >= btn2.y && msg.y <= btn2.y + btn2.height) {
				btn2.change();
				cleardevice();
				return;
			}
		}
	}
}

int main() {
	//程序初始化
	start();
	//加载图像
	loadimage(&mine, "images/mine.png", grid_size, grid_size);
	loadimage(&grid, "images/grid.png", grid_size, grid_size);
	loadimage(&grid_down, "images/grid_down.png", grid_size, grid_size);
	loadimage(&flag, "images/flag.png", grid_size, grid_size);
	//布雷
	vector<vector<Grid>> layout(layout_size+2, vector<Grid>(layout_size+2));//布局，用二维数组，如果是1就有雷，0则没有

	srand(time(nullptr));
	//初始化
	for (int i = 1; i < layout_size+1; i++) {
		for (int j = 1; j < layout_size+1; j++) {
			layout[i][j].init(false, grid_x, grid_y, i, j);
			grid_x += grid_size;
		}
		grid_y += grid_size;
		grid_x = 0;
	}
	//随机布雷实现
	int x = 0, y = 0, count = 0;
	for (count = 0; count < mines_num;){
		x = 1 + rand() % layout_size+1;
		y = 1 + rand() % layout_size+1;
		if (layout[x][y].is_mine == false){
			layout[x][y].is_mine = true;
			count++;//每次布置好一个雷之后,才会计数
		}
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
			for (int i = 1; i < layout_size+1; i++) {
				for (int j = 1; j < layout_size+1; j++) {
					//通过点击坐标判断是哪个格子
					if (msg.x >= layout[i][j].x1 && msg.x <= layout[i][j].x2 && msg.y >= layout[i][j].y1 && msg.y <= layout[i][j].y2) {
						if (layout[i][j].is_click == false) { //判断是否被点击过
							layout[i][j].is_flag = false;
							//如果这个格子是雷
							if (layout[i][j].is_mine) {
								layout[i][j].change(mine);
								//显示提示信息
								for (int x = 0; x < layout_size; x++) {
									for (int y = 0; y < layout_size; y++) {
										if (layout[x][y].is_mine && x != i && y != j)
											layout[x][y].change(mine_other);
									}
								}
								cout << "踩雷！即将退出";
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
						if (all_num == layout_size * layout_size - mines_num) {
							//显示提示信息
							for (int x = 0; x < layout_size; x++) {
								for (int y = 0; y < layout_size; y++) {
									if (layout[x][y].is_mine && x != i && y != j)
										layout[x][y].change(flag);
								}
							}
							cout<<"获胜！即将退出";
							Sleep(2500); //停顿两秒半
							//退出
							return 0;
						}
					}
				}
			}
		}

		else if (msg.message == WM_RBUTTONDOWN) {
			for (int i = 1; i < layout_size+1; i++) {
				for (int j = 1; j < layout_size+1; j++) {
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