#include<iostream>
#include<graphics.h>
#include<vector>
#include<string>
#include<tchar.h>
#include<ctime>

using namespace std;

// 默认常量
int gridSize = 40;									// 每个格子的大小(长和宽)
int lines = 5, rows = 5;							// 行数和列数
int width = rows * gridSize, height = lines * gridSize; // 窗口宽度和高度
int minesNum = 10;									// 雷的数量
int normalGridsNum = lines * rows - minesNum;		// 除了雷以外的格子数量
IMAGE grid, gridClicked, mine, gridFlag;			// 图像

// 格子类
class Grid {
public:
	int posX, posY; // 该格子所处的位置(单位:像素)
	bool isFlag = false, isClicked = false, isMine = false;
	int posLine, posRow; // 该格子所处的行和列位置(单位:格子)
	int minesNumAround = 0; // 该格子周围雷的数量
	
	void show(int posLine,int posRow) {
		//显示
		this->posLine = posLine;
		this->posRow = posRow;
		posX = (this->posRow - 1) * gridSize;
		posY = (this->posLine - 1) * gridSize;
		putimage(posX, posY, &grid);
	}
	void showMinesNumAround() {
		if (minesNumAround==0)
			return;
		else if (minesNumAround == 1)
			settextcolor(BLUE);
		else if (minesNumAround == 2)
			settextcolor(GREEN);
		else if (minesNumAround == 3)
			settextcolor(YELLOW);
		else if (minesNumAround == 4)
			settextcolor(RGB(255, 135, 35));
		else
			settextcolor(RED);
		char info = minesNumAround + 48;
		outtextxy(posX+int(gridSize*0.4), posY+int(gridSize*0.1), _T(info));
	}
	void onLeftButtonClick();
	void onRightButtonClick();
	void findMinesAround();
};

vector<vector<Grid>> playGround(lines+1,vector<Grid>(rows+1));	// 创建整个雷盘
vector<Grid> mines; // 定义一个vector储存是雷的格子

/*定义Grid类的几个函数*/
// 鼠标左键点击
void Grid::onLeftButtonClick() {
	if (!isClicked) { // 如果没有被左键点击过
		isClicked = true;
		normalGridsNum--;
		putimage(posX, posY, &gridClicked);
		
		findMinesAround();		// 找雷数
		showMinesNumAround();	// 显示出来周围雷数

		//翻开周围(4个)不是雷的格子
		vector<vector<int>> minesPosAround = { {posLine - 1,posRow},{posLine,posRow - 1},{posLine,posRow + 1},{posLine + 1,posRow} };
		for (int i = 0; i < 4; i++) {
			if (minesPosAround[i][0] > 0 && minesPosAround[i][0] <= lines && minesPosAround[i][1] > 0 && minesPosAround[i][1] <= rows) {
				if (playGround[minesPosAround[i][0]][minesPosAround[i][1]].isMine == false) {
					playGround[minesPosAround[i][0]][minesPosAround[i][1]].onLeftButtonClick();
				}
			}
		}
	}
}
// 鼠标右键点击
void Grid::onRightButtonClick() {
	if (!isFlag&&!isClicked) { // 如果没有被右键点击过
		isFlag = true;
		putimage(posX, posY, &gridFlag);
	}
	else if(isFlag&&!isClicked) {
		isFlag = false;
		putimage(posX, posY, &grid);
	}
}
// 寻找周围8个(或5个)格子中雷的数量
void Grid::findMinesAround() {
	if (isMine) { // 如果该格子是雷，则返回-1
		return;
	}
	else { // 如果该格子不是雷，继续
		for (int i = posLine - 1; i < posLine + 1; i++) {
			for (int j = posRow - 1; j < posRow + 1; j++) {
				if (i != posLine && j != posRow && playGround[i][j].isMine) {
					minesNumAround++;
				}
			}
		}
	}
}
// 启动画面
void splashScreen() {
	IMAGE splashImage;
	loadimage(&splashImage, "images/mineIcon.png",width,height);
	putimage(0, 0, &splashImage);
	Sleep(1500);

	BeginBatchDraw();
	for (int i = 0; i <= height; i+=3) {
		FlushBatchDraw();
		cleardevice();
		putimage(0, i, &splashImage);
		Sleep(1);
	}
	EndBatchDraw();
	cleardevice();
}
//主函数
int main(int argc, char* argv[]) {
	/*接收来自命令行的参数*/
	if (argc == 2 && argv[1] == "-h") { // 输出帮助信息
		cout << "帮助" << endl;
		cout << "-s:每个格子的大小" << endl;
		cout << "-l:雷盘的行数" << endl << "-r:雷盘的列数" << endl;
		cout << "-n:雷的个数" << endl;
	}
	else if (argc > 2) {
		for (int i = 1; i < argc-1; i++) {
			if (argv[i] == "-s") {
				gridSize = atoi(argv[i + 1]);
			}
			else if (argv[i] == "-l") {
				lines = atoi(argv[i + 1]);
			}
			else if (argv[i] == "-r") {
				rows = atoi(argv[i + 1]);
			}
			else if (argv[i] == "-n") {
				minesNum = atoi(argv[i + 1]);
			}
			else {
				cout << "参数错误" << endl;
			}
		}
	}
	/*初始化画布*/
	initgraph(gridSize * rows, height);
	loadimage(&grid, "images/grid.png", gridSize, gridSize);
	loadimage(&gridClicked, "images/gridClicked.png", gridSize, gridSize);
	loadimage(&mine, "images/mine.png", gridSize, gridSize);
	loadimage(&gridFlag, "images/gridFlag.png", gridSize, gridSize);
	setbkmode(TRANSPARENT);
	settextstyle(int(gridSize*0.9), int(gridSize * 0.4), _T("微软雅黑"));
	setbkcolor(WHITE);
	cleardevice();
	/*启动画面*/
	splashScreen();
	/*布置雷盘*/
	// 显示格子
	for (int i = 1; i <= lines; i++) {
		for (int j = 1; j <= rows; j++) {
			playGround[i][j].show(i, j);
			Sleep(10);
		}
	}
	// 随机布雷
	srand(time(nullptr));
	for (int i = 0; i < minesNum; i++) {
		int x = rand() % rows + 1, y = rand() % lines + 1;
		playGround[x][y].isMine = true;
		mines.push_back(playGround[x][y]);
	}
	/*循环监听鼠标事件*/
	while (true) {
		if (normalGridsNum == 0) {
			// 显示提示信息
			settextcolor(GREEN);

			BeginBatchDraw();
			for (int i = height; i >= 10; i -= 3) {
				FlushBatchDraw();
				cleardevice();
				outtextxy(10, i, "你赢啦");
				Sleep(1);
			}
			EndBatchDraw();

			Sleep(2500);
			return 0;
		}
		ExMessage msg = getmessage(EX_MOUSE);
		if (msg.message == WM_LBUTTONDOWN) {
			for (int i = 1; i <= lines; i++) {
				for (int j = 1; j <= rows; j++) {
					if (msg.x > playGround[i][j].posX&&msg.x< playGround[i][j].posX+gridSize&&msg.y>playGround[i][j].posY&&msg.y< playGround[i][j].posY+gridSize) {
						// 判断点击的是那一个格子
						if (playGround[i][j].isMine) { // 如果是雷
							putimage(playGround[i][j].posX, playGround[i][j].posY, &mine);
							for (int i = 0; i < minesNum; i++) {
								putimage(mines[i].posLine, mines[i].posRow, &mine);
							}
							// 显示提示信息
							settextcolor(RED);
							BeginBatchDraw();
							for (int i = height; i >= 10; i -= 3) {
								FlushBatchDraw();
								cleardevice();
								outtextxy(10, i, "你输啦");
								Sleep(1);
							}
							EndBatchDraw();

							Sleep(2500); // 停顿两秒半
							return 0;
						}
						playGround[i][j].onLeftButtonClick();
					}
				}
			}
		}
		else if (msg.message == WM_RBUTTONDOWN) {
			for (int i = 1; i <= lines; i++) {
				for (int j = 1; j <= rows; j++) {
					if (msg.x > playGround[i][j].posX && msg.x< playGround[i][j].posX + gridSize && msg.y>playGround[i][j].posY && msg.y < playGround[i][j].posY + gridSize) {
						// 判断点击的是那一个格子
						playGround[i][j].onRightButtonClick();
					}
				}
			}
		}
	}
	/*关闭绘图窗口*/
	closegraph();
	return 0;
}