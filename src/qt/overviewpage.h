#ifndef OVERVIEWPAGE_H
#define OVERVIEWPAGE_H

#include "guiutil.h"
#include <QWidget>

namespace Ui {
    class OverviewPage;
}
class ClientModel;
class WalletModel;
class TxViewDelegate;
class TransactionFilterProxy;

QT_BEGIN_NAMESPACE
class QModelIndex;
class UpdateCheck;
QT_END_NAMESPACE

/** Overview ("home") page widget */
class OverviewPage : public QWidget
{
    Q_OBJECT

public:
    explicit OverviewPage(QWidget *parent = 0);
    ~OverviewPage();

    void setClientModel(ClientModel *clientModel);
    void setWalletModel(WalletModel *walletModel);
    void showOutOfSyncWarning(bool fShow);
    void showUpdateWarning(bool fShow);
    void showUpdateLayout(bool fShow);

public slots:
    void setBalance(qint64 balance, qint64 stake, qint64 unconfirmedBalance, qint64 immatureBalance, qint64 mintedBalance);
    void updateValues();

signals:
    void transactionClicked(const QModelIndex &index);
    void valueChanged();

private:
    Ui::OverviewPage *ui;
    ClientModel *clientModel;
    WalletModel *walletModel;
    qint64 currentBalance;
    qint64 currentStake;
    qint64 currentUnconfirmedBalance;
    qint64 currentImmatureBalance;
    qint64 currentTotalBalance;
    qint64 currentMintedBalance;

    TxViewDelegate *txdelegate;
    TransactionFilterProxy *filter;

    void setClientUpdateCheck();
    QLabel *labelUpdateStatic;
    QLabel *labelUpdateStatus;
    UpdateCheck *m_pUpdCtrl;
    QTimer *updateTimer;


    void setPriceUpdateCheck();
    GUIUtil::QCLabel *labelPriceInBTC;
    GUIUtil::QCLabel *labelPriceInUSD;
    GUIUtil::QPriceInfo *priceInfo;

private slots:
    void updateDisplayUnit();
    void handleTransactionClicked(const QModelIndex &index);
    void updateAlerts(const QString &warnings);
    void timerUpdate();
    void checkPrice();
    void checkForUpdates();
};

#endif // OVERVIEWPAGE_H
