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
    std::string state; //������� ��� �� ������� �� �������,���������� ��� �� ����������
public:
    ModelSignal(std::string ipaddr,int port){InitSocket(ipaddr,port);}
    ~ModelSignal(){}
public:
    void UpdateSignalConnect() override
    {
        //�������� ����������� ������ �� ��������� send
        if(state == "send")
        {
            this->Send(mSocket->uSend);
        }
        //����� ����������� ������
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

        #ifdef _WIN32
            // Windows: используем ioctlsocket вместо fcntl
            u_long mode = nonBlocking ? 1 : 0;
            if (ioctlsocket(mSocket->socket, FIONBIO, &mode) != 0) {
                printf("WaitConnect: failed to set non-blocking socket\n"); 
                usleep(2000);
                exit(0);
            }
        #else
            if(fcntl(mSocket->socket,F_SETFL,O_NONBLOCK,nonBlocking)==-1)
            {
                printf("WaitConnect: failed to set non-blocking socket\n"); 
                usleep(2000);
                exit(0);
            }
        #endif
    }
    void Send(uint8_t *usend)
    {
        #ifdef _WIN32
            // Для Windows - приводим к const char*
            int sb = send(mSocket->socket, (const char*)usend, sizeof(usend), 0);
        #else
            // Для Linux - можно использовать const void*
            int sb = send(mSocket->socket, &usend, sizeof(usend), 0);
        #endif
    }
    void Send(std::vector<uint8_t> vustr)
    {
        #ifdef _WIN32
            int sb = send(mSocket->socket, 
                        reinterpret_cast<const char*>(vustr.data()), 
                        vustr.size(), 
                        0);
        #else
            int sb = send(mSocket->socket,&vustr,sizeof(vustr),0);
        #endif
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
    std::string state; //������� ��� �� ������� �� �������
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
    //���� ��� �� ������, �������� �� � ���������� ������
    //��� ��� ��� ������, ����������� �� ��� ��� ���� �� �����������
    void WaitConnect()
    {
        int con = connect(mSocket->socket,
            reinterpret_cast<const sockaddr*>(&mSocket->sockaddr),
            sizeof(sockaddr));
        int nonBlocking = 1;

        #ifdef _WIN32
            // Windows: используем ioctlsocket вместо fcntl
            u_long mode = nonBlocking ? 1 : 0;
            if (ioctlsocket(mSocket->socket, FIONBIO, &mode) != 0) {
                printf("WaitConnect: failed to set non-blocking socket\n"); 
                usleep(2000);
                exit(0);
            }
        #else
            if(fcntl(mSocket->socket,F_SETFL,O_NONBLOCK,nonBlocking)==-1)
            {
                printf("WaitConnect: failed to set non-blocking socket\n"); 
                usleep(2000);
                exit(0);
            }
        #endif
    }
    void Send(uint8_t *usend)
    {
        #ifdef _WIN32
            // Для Windows - приводим к const char*
            int sb = send(mSocket->socket, (const char*)usend, sizeof(usend), 0);
        #else
            // Для Linux - можно использовать const void*
            int sb = send(mSocket->socket, &usend, sizeof(usend), 0);
        #endif
    }
    void Send(std::vector<uint8_t> vustr)
    {
        #ifdef _WIN32
            int sb = send(mSocket->socket, 
                        reinterpret_cast<const char*>(vustr.data()), 
                        vustr.size(), 
                        0);
        #else
            int sb = send(mSocket->socket,&vustr,sizeof(vustr),0);
        #endif
    }
    //������ �������� � ����������� �����
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

    std::string state; //������� ��� �� ������� �� �������
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
    std::string state; //������� ��� �� ������� �� �������
public:
    ModelTimer(){}
    ~ModelTimer(){}
public:
    void UpdateTimer()override{}
};

//��������� ������� ��� �������,�����,�������,�������
class ModelThread
{
private:
    //������� ��� ��������� ����������� �����
    std::vector<IUpdateSignalConnect*> vSignal;
    std::vector<IUpdateMediaConnect*> vMedia;
    std::vector<IUpdateEvent*> vEvent;
    std::vector<IUpdateTimer*> vTimer;
    //����� � ������� ������������ tcp-signal �����������
    pthread_t thsignal;
    //����� � ������� ������������ udp-media �����������
    pthread_t thmedia;
    //����� � ������� �������������� �������
    pthread_t thevent;
    //����� � ������� �������������� �������
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
        //��������� � ��������� ������
        while(1)
        {
            usleep(10);
            //������������ ������� ���������� ����������
            if (vSignal.size() > 0)
            {
                for (int i = 0; i < (int)(vSignal.size()); i++)
                {
                    //�������� ����������� ������ � ��������� send
                    //����� ����������� ���������
                    //�������� ���������� �������
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
        //��������� � ��������� ������
        while(1)
        {
            usleep(10);
            //������������ ������� ����� ����������
            if (vMedia.size() > 0)
            {   
                for (int i = 0; i < (int)(vMedia.size()); i++)
                {
                    //�������� ���������� �����
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
        //��������� � ��������� ������
        while(1)
        {
            usleep(10);
            //������������ ������� �������
            if (vEvent.size() > 0)
            {
                for (int i = 0; i < (int)(vEvent.size()); i++)
                {
                    //�������� ���������� �������
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
        //��������� � ��������� ������
        while(1)
        {
            usleep(10);
            //������������ ������� �������
            if (vTimer.size() > 0)
            {
                for (int i = 0; i < (int)(vTimer.size()); i++)
                {
                    //�������� ���������� �������
                    vTimer[i]->UpdateTimer();
                }
                vTimer.clear();
            }
        }
    }
    //��������� �������
    void SetSignal(IUpdateSignalConnect *msignal){vSignal.push_back(msignal);}
    void SetMedia(IUpdateMediaConnect *mmedia){vMedia.push_back(mmedia);}
    void SetEvent(IUpdateEvent *mevent){vEvent.push_back(mevent);}
    void SetTimer(IUpdateTimer *mtimer){vTimer.push_back(mtimer);}
};

//������� ������ �������� ��� ����� � �������
class ModelBase
{
public:
    ModelBase(){}
    ~ModelBase(){}
};

//��������� ��� �������� ������ �� ������� �� ������
class ISendSignal
{
public:
    virtual void SendSignal(std::vector<uint8_t> vusend) = 0;
};

//������ ����������� ������ ������ �������� ������� � ��� ����������
//�� ��� ��� � ����������� ������� ����� ���������, ���������� ����� ������� � ������� �������
class ModelConnect : public ModelBase, public ISendSignal
{
private:
    //������ ������� �������
    ModelThread *mThread;
    //���� ���������� ���������� (��� ����������,������ ����������)
    std::unordered_map<std::string,ModelSignal*> umapSignal;
    std::unordered_map<std::string,ModelMedia*> umapMedia;
public:
    //��������� �������� ����� � ����
    ModelConnect(std::string ipaddr,int port)
    {
        //������� ������� �������
        mThread = new ModelThread();
        //������� ���������� �����
        umapSignal.emplace("main",new ModelSignal(ipaddr,port));
        //vSignal.push_back(new ModelSignal("172.18.3.147",32895));
        //������ ���������� ����� � ���������� ������� ������� ��� �������� ��������� //ConnectSignal()
        this->ConnectSignal("main");
        //���� ��� ��� �� ������, ���������� ���� �� ��������� ������� ���������� ���������� �� ������ � ��������� �����
    }
    ~ModelConnect(){}
public:
    //������������ ��������� ����������� �����������
    //void ConnectSignal(IUpdateSignalConnect const & signalConnect)
    void ConnectSignal(std::string name)
    {
        //���������� ���������� ������ � �������
        mThread->SetSignal(umapSignal.find(name)->second);
    }
    //������������ �������� ����� �����������
    //void ConnectMedia(IUpdateMediaConnect const & mediaConnect)
    void ConnectMedia()
    {

    }
    //������������ ��������� �������
    //void Event(IUpdateEvent const & event)
    void Event()
    {

    }
    //������������ �������� �������
    //void Timer(IUpdateTimer const & timer)
    void Timer()
    {
        
    }
    //��������� ��������� ��������� ������ �� ������� �������
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