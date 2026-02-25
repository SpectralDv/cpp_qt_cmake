#ifndef MODELCONNECT_H
#define MODELCONNECT_H

#include "Include.h"

class IUpdateSignalConnect
{
public:
    virtual void UpdateSignalConnect() = 0;
};

class IUpdateMediaConnect
{
public:
    virtual void UpdateMediaConnect() = 0;
};

class IUpdateEvent
{
public:
    virtual void UpdateEvent() = 0;
};

class IUpdateTimer
{
public:
    virtual void UpdateTimer() = 0;
};

class ModelSocket
{
public: 
    int sizeData = 32;
    uint8_t *uSend;
    std::string ipaddr;
    int port;
    int socket;
    struct sockaddr_in sockaddr;
    int sockaddr_size;
public:
    ModelSocket(){}
    ~ModelSocket(){}
};


class ModelSignal : public IUpdateSignalConnect
{
private: //tcp
    ModelSocket *mSocket;
    std::string state; //удалять или не удалять из очереди,отправлять или не отправлять
public:
    ModelSignal(std::string ipaddr,int port){InitSocket(ipaddr,port);}
    ~ModelSignal(){}
public:
    void UpdateSignalConnect() override
    {
        //отправка выполняется только из состояния send
        if(state == "send")
        {
            this->Send(mSocket->uSend);
        }
        //прием выполняется всегда
        Recv();
    }
    void InitSocket(std::string ipAddr,int port)
    {
        mSocket = new ModelSocket();
		mSocket->socket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
		mSocket->sockaddr.sin_family = AF_INET; //AF_INET6
		mSocket->sockaddr.sin_port = htons(port);
        mSocket->sockaddr.sin_addr.s_addr = inet_addr(ipAddr.c_str());
        mSocket->sockaddr_size = sizeof(mSocket->sockaddr);
    }
    void WaitConnect()
    {
        int con = connect(mSocket->socket,
            reinterpret_cast<const sockaddr*>(&mSocket->sockaddr),
            sizeof(sockaddr));
        if(con!=0){printf("Server disconnect\n");usleep(2000);exit(0);}

        int nonBlocking = 1;
        if(fcntl(mSocket->socket,F_SETFL,O_NONBLOCK,nonBlocking)==-1)
        {
            printf("WaitConnect: failed to set non-blocking socket\n"); 
            usleep(2000);
            exit(0);
        }
    }
    void Send(uint8_t *usend)
    {
        int sb = send(mSocket->socket,&usend,sizeof(usend),0);
    }
    void Send(std::vector<uint8_t> vustr)
    {
        int sb = send(mSocket->socket,&vustr,sizeof(vustr),0);
    }
    void Recv()
    {
        char *cstr;
        int rb = recv(mSocket->socket,cstr,sizeof(cstr),0);
        //rb = recvfrom(sRecv, (char*)&rdata, sizeof(rdata), 0, (SOCKADDR*)&addrRecv, &sizeRecv);
        if(rb>0)
        {
            printf(": %s\n",cstr);
        }
    }
};

class ModelMedia : public IUpdateMediaConnect
{
private://udp
    ModelSocket *mSocket;
    std::string state; //удалять или не удалять из очереди
public:
    ModelMedia(){}
    ~ModelMedia(){}
public:
    void UpdateMediaConnect()override{}
    void InitSocket(std::string ipAddr,int port)
    {
        mSocket = new ModelSocket();
		mSocket->socket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
		mSocket->sockaddr.sin_family = AF_INET; //AF_INET6
		mSocket->sockaddr.sin_port = htons(mSocket->port);       
        mSocket->sockaddr.sin_addr.s_addr = inet_addr(ipAddr.c_str());   
        mSocket->sockaddr_size = sizeof(mSocket->sockaddr);   
    }
    //если был бы сервер, крутился бы в бесконечно циклеы
    //так как это клиент, выполняется до тех пор пока не подключится
    void WaitConnect()
    {
        int con = connect(mSocket->socket,
            reinterpret_cast<const sockaddr*>(&mSocket->sockaddr),
            sizeof(sockaddr));

        int nonBlocking = 1;
        if(fcntl(mSocket->socket,F_SETFL,O_NONBLOCK,nonBlocking)==-1)
        {
            printf("WaitConnect: failed to set non-blocking socket\n"); 
            usleep(2000);
            exit(0);
        }
    }
    void Send(uint8_t *usend)
    {
        int sb = send(mSocket->socket,&usend,sizeof(usend),0);
    }
    void Send(std::vector<uint8_t> vustr)
    {
        int sb = send(mSocket->socket,&vustr,sizeof(vustr),0);
    }
    //должен работать в бесконечном цикле
    void Recv()
    {
        char *cstr;
        int rb = recv(mSocket->socket,cstr,sizeof(cstr),0);
        //rb = recvfrom(sRecv, (char*)&rdata, sizeof(rdata), 0, (SOCKADDR*)&addrRecv, &sizeRecv);
        if(rb>0)
        {
            printf(": %s\n",cstr);
        }
    }
};

class ModelEvent : public IUpdateEvent
{
private:

    std::string state; //удалять или не удалять из очереди
public:
    ModelEvent(){}
    ~ModelEvent(){}
public:
    void UpdateEvent()override{}
};

class ModelTimer : public IUpdateTimer
{
private:
    int startTimer;
    int endTimer;
    std::string state; //удалять или не удалять из очереди
public:
    ModelTimer(){}
    ~ModelTimer(){}
public:
    void UpdateTimer()override{}
};

//потоковые очереди для сигнала,медиа,события,таймера
class ModelThread
{
private:
    //очереди для обработки интерфейсов извне
    std::vector<IUpdateSignalConnect*> vSignal;
    std::vector<IUpdateMediaConnect*> vMedia;
    std::vector<IUpdateEvent*> vEvent;
    std::vector<IUpdateTimer*> vTimer;
    //поток в котором производится tcp-signal подключение
    pthread_t thsignal;
    //поток в котором производится udp-media подключение
    pthread_t thmedia;
    //поток в котором обрабатываются события
    pthread_t thevent;
    //поток в котором обрабатываются таймеры
    pthread_t thtimer;
public:
    ModelThread()
    {
        pthread_create(&thsignal,NULL,ThreadUpdateSignalConnect,(void*)this);
        pthread_detach(thsignal);
        pthread_create(&thmedia,NULL,ThreadUpdateMediaConnect,(void*)this);
        pthread_detach(thmedia);
        pthread_create(&thevent,NULL,ThreadUpdateEvent,(void*)this);
        pthread_detach(thevent);
        pthread_create(&thtimer,NULL,ThreadUpdateTimer,(void*)this);
        pthread_detach(thtimer);
    }
    ~ModelThread(){}
public:
    static void *ThreadUpdateSignalConnect(void* self)
    {
		((ModelThread*)self)->UpdateSignalConnect();
		return 0;
    }
    void UpdateSignalConnect()
    {
        //крутиться в отдельном потоке
        while(1)
        {
            usleep(10);
            //обрабатывает очередь сигнальных соединений
            if (vSignal.size() > 0)
            {
                for (int i = 0; i < (int)(vSignal.size()); i++)
                {
                    //отправка выполняется только в состоянии send
                    //прием выполняется постоянно
                    //вызывает обработчик сигнала
                    vSignal[i]->UpdateSignalConnect();
                }
                //vSignal.clear();
            }
        }
    }
    static void *ThreadUpdateMediaConnect(void* self)
    {
		((ModelThread*)self)->UpdateMediaConnect();
		return 0;
    }
    void UpdateMediaConnect()
    {
        //крутиться в отдельном потоке
        while(1)
        {
            usleep(10);
            //обрабатывает очередь медиа соединений
            if (vMedia.size() > 0)
            {   
                for (int i = 0; i < (int)(vMedia.size()); i++)
                {
                    //вызывает обработчик медиа
                    vMedia[i]->UpdateMediaConnect();
                }
                //vMedia.clear();
            } 
        }
    }
    static void *ThreadUpdateEvent(void* self)
    {
		((ModelThread*)self)->UpdateMediaConnect();
		return 0;
    }
    void UpdateEvent()
    {
        //крутиться в отдельном потоке
        while(1)
        {
            usleep(10);
            //обрабатывает очередь событий
            if (vEvent.size() > 0)
            {
                for (int i = 0; i < (int)(vEvent.size()); i++)
                {
                    //вызывает обработчик события
                    vEvent[i]->UpdateEvent();
                }
                vEvent.clear();
            } 
        }
    }
    static void *ThreadUpdateTimer(void* self)
    {
		((ModelThread*)self)->UpdateMediaConnect();
		return 0;
    }
    void UpdateTimer()
    {
        //крутиться в отдельном потоке
        while(1)
        {
            usleep(10);
            //обрабатывает очередь темеров
            if (vTimer.size() > 0)
            {
                for (int i = 0; i < (int)(vTimer.size()); i++)
                {
                    //вызывает обработчик таймера
                    vTimer[i]->UpdateTimer();
                }
                vTimer.clear();
            }
        }
    }
    //заполняет очереди
    void SetSignal(IUpdateSignalConnect *msignal){vSignal.push_back(msignal);}
    void SetMedia(IUpdateMediaConnect *mmedia){vMedia.push_back(mmedia);}
    void SetEvent(IUpdateEvent *mevent){vEvent.push_back(mevent);}
    void SetTimer(IUpdateTimer *mtimer){vTimer.push_back(mtimer);}
};

//базовая модель например для связи с кнопкой
class ModelBase
{
public:
    ModelBase(){}
    ~ModelBase(){}
};

//интерфейс для отправки пакета по нажатию на кнопку
class ISendSignal
{
public:
    virtual void SendSignal(std::vector<uint8_t> vusend) = 0;
};

//модель подключений хранит модель очередей потоков и все соединения
//но так как в соединениях слушать нужно постоянно, соединения можно хранить в очереди потоков
class ModelConnect : public ModelBase, public ISendSignal
{
private:
    //модель очереди потоков
    ModelThread *mThread;
    //мапа сигнальных соединений (имя устройства,модель соединения)
    std::unordered_map<std::string,ModelSignal*> umapSignal;
    std::unordered_map<std::string,ModelMedia*> umapMedia;
public:
    //принимает основной адрес и порт
    ModelConnect(std::string ipaddr,int port)
    {
        //создает очередь потоков
        mThread = new ModelThread();
        //создает сигнальный сокет
        umapSignal.emplace("main",new ModelSignal(ipaddr,port));
        //vSignal.push_back(new ModelSignal("172.18.3.147",32895));
        //кладет сигнальный сокет в сигнальную очередь потоков для отправки сообщения //ConnectSignal()
        this->ConnectSignal("main");
        //если это был бы сервер, необходимо было бы заполнять очередь сигнальных соединений из чтения с основгого порта
    }
    ~ModelConnect(){}
public:
    //обрабатывает интерфейс сигнального подключения
    //void ConnectSignal(IUpdateSignalConnect const & signalConnect)
    void ConnectSignal(std::string name)
    {
        //отправляет сигнальную модель в очередь
        mThread->SetSignal(umapSignal.find(name)->second);
    }
    //обрабатывает интефейс медиа подключения
    //void ConnectMedia(IUpdateMediaConnect const & mediaConnect)
    void ConnectMedia()
    {

    }
    //обрабатывает интерфейс события
    //void Event(IUpdateEvent const & event)
    void Event()
    {

    }
    //обрабатывает интефейс таймера
    //void Timer(IUpdateTimer const & timer)
    void Timer()
    {
        
    }
    //отправить сообщение выбранной модели из очереди потоков
    void SendSignal(std::vector<uint8_t> vusend) override
    {
        umapSignal.find("main")->second->Send(vusend);
    }
};



class ViewConnect
{
public:
    ViewConnect(){}
    ~ViewConnect(){}
public:
    void ConnectInit()
    {

    }
};


#endif 