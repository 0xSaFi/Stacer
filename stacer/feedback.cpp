#include "feedback.h"
#include "ui_feedback.h"
#include "Utils/command_util.h"

#include <QJsonDocument>
#include <QJsonObject>
#include <QDebug>
#include <QtConcurrent>

Feedback::~Feedback()
{
    delete ui;
}

Feedback::Feedback(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Feedback),
    header("Content-Type: application/json"),
    feedbackUrl("https://stacer-web-api.herokuapp.com/feedback"),
    mailRegex("\\b[A-Z0-9._%+-]+@[A-Z0-9.-]+\\.[A-Z]{2,4}\\b")
{
    ui->setupUi(this);

    init();
}

void Feedback::init()
{
    mailRegex.setCaseSensitivity(Qt::CaseInsensitive);
    mailRegex.setPatternSyntax(QRegExp::RegExp);

    connect(this, &Feedback::clearInputsS, this, &Feedback::clearInputs);
    connect(this, &Feedback::setErrorMessageS, this, &Feedback::setErrorMessage);
    connect(this, &Feedback::disableElementsS, this, &Feedback::disableElements);
}

void Feedback::on_sendBtn_clicked()
{
    QString name = ui->nameTxt->text();
    QString email = ui->emailTxt->text();
    QString message = ui->messageTxt->toPlainText();

    bool isEmailValid = mailRegex.exactMatch(email);

    if (! isEmailValid) {
        emit setErrorMessageS(tr("Email address is not valid !"));
        return;
    }

    if (message.length() < 25) {
        emit setErrorMessageS(tr("Your message must be at least 25 characters !"));
        return;
    }

    if (! name.isEmpty() &&
        ! email.isEmpty() && isEmailValid)
    {
        QtConcurrent::run([=] {
            emit disableElementsS(true);

            ui->sendBtn->setText(tr("Sending.."));
            QStringList args;

            QJsonObject postData;
            postData["name"] = name;
            postData["email"] = email;
            postData["message"] = message;

            QJsonDocument json(postData);

            args << "-d" << json.toJson() << "-H" << header << "-X" << "POST" << feedbackUrl;

            try {
                QString result = CommandUtil::exec("curl", args);
                QJsonObject response = QJsonDocument::fromJson(result.toUtf8()).object();

                if (response.value("success").toBool()) {
                    emit clearInputs();
                    emit setErrorMessageS(tr("<font color='#2ecc71'>Your Feedback has been successfully sended.</font>"));
                } else {
                    emit setErrorMessageS(tr("Something went wrong, try again !"));
                }

            } catch(QString &ex) {
                qCritical() << ex;
                emit setErrorMessageS(tr("Something went wrong, try again !"));
            }

            ui->sendBtn->setText(tr("Save"));
            emit disableElementsS(false);
        });

    } else {
        emit setErrorMessageS(tr("Fields cannot be left blank !"));
    }
}

void Feedback::setErrorMessage(const QString &msg)
{
    ui->lblErrorMsg->setText(msg);
}

void Feedback::disableElements(const bool status)
{
    ui->nameTxt->setDisabled(status);
    ui->emailTxt->setDisabled(status);
    ui->messageTxt->setDisabled(status);
    ui->sendBtn->setDisabled(status);
}

void Feedback::clearInputs()
{
    ui->nameTxt->clear();
    ui->emailTxt->clear();
    ui->messageTxt->clear();
    ui->nameTxt->setFocus();
}
