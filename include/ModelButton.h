#ifndef MODELBUTTON_H
#define MODELBUTTON_H



#include "ModelConnect.h"


//интерфейс нажатия кнопки
class IPushButton
{
public:
    virtual void PushButton() = 0;
}; 

//интерфейс подключения кнопки и наследника базовой модели
class IConnectButton
{
public:
    virtual void ConnectButton(ModelBase *mbase) = 0;
};

//интерфейс для вызова метода из наследника базовой модели в кнопке


//базов класс кнопки
class ModelButton : public IPushButton, public QObject
{
protected:
    QPushButton *button;
protected:
    ModelButton(){}
public:
    ModelButton(QPushButton *_button,const int (&point)[4])
    {
        button = _button;
        button->setGeometry(QRect(QPoint(point[0], point[1]), QSize(point[2], point[3])));
        connect(button, &QPushButton::released, this, &ModelButton::PushButton);
    }
    ~ModelButton(){}
public:
    void PushButton() override
    {
        //std::cout<<"push button: "<< (button->objectName()).toStdString() << std::endl;
        std::cout<<"push button: "<< (button->text()).toStdString() << std::endl;
    }
};

class ModelButtonInit : public ModelButton, public IConnectButton
{
private:
    ModelBase *mBase;
public:
    ModelButtonInit(QPushButton *_button,const int (&point)[4])
    {
        button = _button;
        button->setGeometry(QRect(QPoint(point[0], point[1]), QSize(point[2], point[3])));
        connect(button, &QPushButton::released, this, &ModelButton::PushButton);
    }
    ~ModelButtonInit(){}
public:
    //обработка нажатия кнопки
    void PushButton() override
    {
        //отправляет сообщение инициализации на сервер
        std::vector<uint8_t> vuSend{0x00,0x0f,0xff};
        ((ISendSignal*)(mBase))->SendSignal(vuSend);
        std::cout<<"push button: "<< "ModelButtonInit" << std::endl;
    }
    //присваивает модель с которой будет работать при вызове
    void ConnectButton(ModelBase *mbase) override
    {
        mBase = mbase;
    }
};


#endif