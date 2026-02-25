
#include "MainWindow.h"


MainWindow::MainWindow(QWidget *parent) :
QMainWindow(parent),
ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    this->setWindowTitle("QTPHONE");
    //this->setWindowFlags(Qt::Dialog | Qt::MSWindowsFixedSizeDialogHint);
    //myWidget->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    this->setFixedSize(this->size());
    this->setWindowFlags(windowType() | Qt::WindowStaysOnTopHint); //| Qt::FramelessWindowHint

    mapButton.emplace("button1",new ModelButton(new QPushButton("1", this),{350,400,50,50}));
    mapButton.emplace("button2",new ModelButton(new QPushButton("2", this),{400,400,50,50}));
    mapButton.emplace("button3",new ModelButton(new QPushButton("3", this),{450,400,50,50}));
    mapButton.emplace("button4",new ModelButton(new QPushButton("4", this),{350,450,50,50}));
    mapButton.emplace("button5",new ModelButton(new QPushButton("5", this),{400,450,50,50}));
    mapButton.emplace("button6",new ModelButton(new QPushButton("6", this),{450,450,50,50}));
    mapButton.emplace("button7",new ModelButton(new QPushButton("7", this),{350,500,50,50}));
    mapButton.emplace("button8",new ModelButton(new QPushButton("8", this),{400,500,50,50}));
    mapButton.emplace("button9",new ModelButton(new QPushButton("9", this),{450,500,50,50}));

    mapButton.emplace("button10",new ModelButton(new QPushButton("<-", this),{150,450,50,50}));
    mapButton.emplace("button11",new ModelButton(new QPushButton("ok", this),{200,450,50,50}));
    mapButton.emplace("button12",new ModelButton(new QPushButton("->", this),{250,450,50,50}));
    mapButton.emplace("button13",new ModelButton(new QPushButton("/\\", this),{200,400,50,50}));
    mapButton.emplace("button14",new ModelButton(new QPushButton("\\/", this),{200,500,50,50}));


    mapButton.emplace("button15",new ModelButton(new QPushButton("*--*", this),{150,280,50,50}));
    mapButton.emplace("button16",new ModelButton(new QPushButton("->->", this),{200,280,50,50}));
    mapButton.emplace("button17",new ModelButton(new QPushButton("*->", this),{250,280,50,50}));
    mapButton.emplace("button18",new ModelButton(new QPushButton("[V]", this),{170,330,50,50}));
    mapButton.emplace("button19",new ModelButton(new QPushButton("|||", this),{230,330,50,50}));

    mapButton.emplace("button20",new ModelButton(new QPushButton("./\\.", this),{370,280,50,50}));
    mapButton.emplace("button21",new ModelButton(new QPushButton(".\\*", this),{430,280,50,50}));
    mapButton.emplace("button22",new ModelButton(new QPushButton("--", this),{350,330,50,50}));
    mapButton.emplace("button23",new ModelButton(new QPushButton("D))", this),{400,330,50,50}));
    mapButton.emplace("button24",new ModelButton(new QPushButton("+", this),{450,330,50,50}));

    mapButton.emplace("button25",new ModelButton(new QPushButton("--", this),{650,0,50,50}));
    mapButton.emplace("button26",new ModelButton(new QPushButton("--", this),{650,50,50,50}));
    mapButton.emplace("button27",new ModelButton(new QPushButton("--", this),{650,100,50,50}));
    mapButton.emplace("button28",new ModelButton(new QPushButton("--", this),{650,150,50,50}));
    mapButton.emplace("button29",new ModelButton(new QPushButton("--", this),{650,200,50,50}));
    mapButton.emplace("button30",new ModelButton(new QPushButton("--", this),{650,250,50,50}));

    mapButton.emplace("button31",new ModelButton(new QPushButton("Up", this),{50,400,50,50}));
    mapButton.emplace("button32",new ModelButton(new QPushButton("Down", this),{50,450,50,50}));

    mapButton.emplace("buttonInit",new ModelButtonInit(new QPushButton("Init", this),{50,300,50,50}));


    //������� ������ �����������
    ModelConnect *mConnect = new ModelConnect("127.0.0.1",32895);
    
    //� ������ ������ �������� ������ � ������� �� ������ ��������
    ((ModelButtonInit*)(mapButton.find("buttonInit")->second))->ConnectButton(mConnect);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::paintEvent(QPaintEvent *e)
{
    QPainter painter;//(this);
    painter.begin(this);

    //���
    painter.setBrush(QBrush(Qt::black, Qt::SolidPattern));
    painter.drawRect(0,0,800,700);

    //�����
    painter.setPen(QPen(Qt::white, 1, Qt::SolidLine, Qt::FlatCap));
    painter.drawRect(10,10,600,250);

    //�������� ������
    painter.drawLine(400,10,400,260);
    painter.drawLine(10,50,400,50);
    painter.drawLine(10,90,30,90);
    painter.drawLine(10,130,30,130);
    painter.drawLine(10,170,30,170);
    painter.drawLine(10,210,400,210);

    //painter.drawEllipse(100, 50, 150, 150);
    //painter.drawPolygon(QPolygon()<<QPoint(5, 20)<<QPoint(35, 40)<<QPoint(40, 100));
    painter.end();
}


