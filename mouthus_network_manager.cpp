#include "mouthus_network_manager.h"

#include <QDebug>

MouthusNetworkManager::MouthusNetworkManager(QObject* parent) :
    _url(""),
    _networkAccessManager(new QNetworkAccessManager)
{
    connect(_networkAccessManager, SIGNAL(finished(QNetworkReply*)), this, SLOT(handleNetworkReply(QNetworkReply*)));
}

MouthusNetworkManager::~MouthusNetworkManager()
{
}

void MouthusNetworkManager::sendHttpRequest()
{
    Q_ASSERT(_url!="");
    _networkAccessManager->get(QNetworkRequest(QUrl(_url)));
}

void MouthusNetworkManager::handleNetworkReply(QNetworkReply* reply)
{
    if(reply->error() == QNetworkReply::NoError)
    {
        QString replyString = reply->readAll();
        
        qDebug() << "replyString :" << replyString;
        
        if(replyString == "transmission ok")
            emit httpReplyReceived(tr("Les statistiques ont ete transmises"));
        else
            emit httpReplyReceived(tr("Les statistiques n'ont pas ete transmises..."));
    }
}

void MouthusNetworkManager::setUrl(const QString & url)
{
    _url = url;
}