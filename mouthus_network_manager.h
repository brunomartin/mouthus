#ifndef MOUTHUS_NETWORK_MANAGER_H
#define MOUTHUS_NETWORK_MANAGER_H

#include <QtNetwork>
#include <QtCore>

class QNetworkAccessManager;

class MouthusNetworkManager : public QObject
{
    Q_OBJECT
public:
        
    MouthusNetworkManager(QObject* parent = 0);
    ~MouthusNetworkManager();
    
protected:
    
private:
    QString _url;
    QNetworkAccessManager* _networkAccessManager;
    
signals:
    void httpReplyReceived(const QString & reply);
    
public slots:
    void setUrl(const QString & url);
    void sendHttpRequest();
    void handleNetworkReply(QNetworkReply* reply);
    
};

#endif //MOUTHUS_NETWORK_MANAGER_H