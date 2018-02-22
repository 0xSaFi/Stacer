#include "apt_source_manager_page.h"
#include "ui_apt_source_manager_page.h"
#include <QDebug>

#include "Managers/tool_manager.h"

APTSourceManagerPage::~APTSourceManagerPage()
{
    delete ui;
}

APTSourcePtr APTSourceManagerPage::selectedAptSource = nullptr;

APTSourceManagerPage::APTSourceManagerPage(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::APTSourceManagerPage),
    fileSystemWatcher(this)
{
    ui->setupUi(this);

    init();
}

void APTSourceManagerPage::init()
{
    fileSystemWatcher.addPaths({ APT_SOURCES_LIST_D_PATH, APT_SOURCES_LIST_PATH });
    connect(&fileSystemWatcher, &QFileSystemWatcher::directoryChanged, this, &APTSourceManagerPage::loadAptSources);
    connect(&fileSystemWatcher, &QFileSystemWatcher::fileChanged, this, &APTSourceManagerPage::loadAptSources);

    ui->txtAptSource->setPlaceholderText(tr("example %1").arg("'deb http://archive.ubuntu.com/ubuntu xenial main'"));

    loadAptSources();

    on_btnCancel_clicked();
}

void APTSourceManagerPage::loadAptSources()
{
    ui->aptSourceRepositoryListWidget->clear();

    QList<APTSourcePtr> aptSourceList = ToolManager::ins()->getSourceList();

    for (APTSourcePtr &aptSource: aptSourceList) {

        QListWidgetItem *listItem = new QListWidgetItem(ui->aptSourceRepositoryListWidget);
        listItem->setData(5, aptSource->source); // for search

        APTSourceRepositoryItem *aptSourceItem = new APTSourceRepositoryItem(aptSource, ui->aptSourceRepositoryListWidget);

        listItem->setSizeHint(aptSourceItem->sizeHint());

        ui->aptSourceRepositoryListWidget->setItemWidget(listItem, aptSourceItem);
    }

    ui->notFoundWidget->setVisible(aptSourceList.isEmpty());

    ui->aptSourceTitleLbl->setText(tr("APT Source Repositories (%1)")
                                   .arg(aptSourceList.count()));
}

void APTSourceManagerPage::on_btnAddAPTSourceRepository_clicked(bool checked)
{
    if (checked) {
        ui->btnAddAPTSourceRepository->setText(tr("Save"));
        changeElementsVisible(checked);
    }
    else {
        QString aptSourceRepository = ui->txtAptSource->text().trimmed();

        if (!aptSourceRepository.isEmpty()) {
            QStringList args = { "-y", aptSourceRepository };
            if (ui->checkEnableSource->isChecked()) {
                args << "-s";
            }

            CommandUtil::sudoExec("add-apt-repository", args);

            ui->txtAptSource->clear();
            ui->checkEnableSource->setChecked(false);
            on_btnCancel_clicked();
        }
    }
}

void APTSourceManagerPage::on_btnCancel_clicked()
{
    ui->btnAddAPTSourceRepository->setChecked(false);
    changeElementsVisible(false);
    ui->btnAddAPTSourceRepository->setText(tr("Add APT Source"));
}

void APTSourceManagerPage::changeElementsVisible(const bool checked)
{
    ui->txtAptSource->setVisible(checked);
    ui->checkEnableSource->setVisible(checked);
    ui->btnCancel->setVisible(checked);
    ui->btnEditAptSource->setVisible(!checked);
    ui->btnDeleteAptSource->setVisible(!checked);
    if (checked)
        ui->bottomSectionHorizontalSpacer->changeSize(0, 0, QSizePolicy::Minimum);
    else
        ui->bottomSectionHorizontalSpacer->changeSize(0, 0, QSizePolicy::Expanding);
}

void APTSourceManagerPage::on_aptSourceRepositoryListWidget_itemClicked(QListWidgetItem *item)
{
    QWidget *widget = ui->aptSourceRepositoryListWidget->itemWidget(item);
    if (widget) {
        APTSourceRepositoryItem *aptSourceItem = dynamic_cast<APTSourceRepositoryItem*>(widget);
        if (aptSourceItem) {
            selectedAptSource = aptSourceItem->aptSource();
        }
    } else {
        selectedAptSource.clear();
    }
}

void APTSourceManagerPage::on_aptSourceRepositoryListWidget_itemDoubleClicked(QListWidgetItem *item)
{
    on_aptSourceRepositoryListWidget_itemClicked(item);
    on_btnEditAptSource_clicked();
}

void APTSourceManagerPage::on_btnDeleteAptSource_clicked()
{
    if (! selectedAptSource.isNull()) {
        ToolManager::ins()->removeAPTSource(selectedAptSource->source);
    }
}

void APTSourceManagerPage::on_txtSearchAptSource_textChanged(const QString &val)
{
    for (int i = 0; i < ui->aptSourceRepositoryListWidget->count(); ++i) {
        QListWidgetItem *item = ui->aptSourceRepositoryListWidget->item(i);
        if (item) {
            bool isContain = item->data(5).toString().contains(val, Qt::CaseInsensitive);
            ui->aptSourceRepositoryListWidget->setItemHidden(item, ! isContain);
        }
    }
}

void APTSourceManagerPage::on_btnEditAptSource_clicked()
{
    if (! selectedAptSource.isNull()) {
        if (aptSourceEditDialog.isNull()) {
            aptSourceEditDialog = QSharedPointer<APTSourceEdit>(new APTSourceEdit(this));
        }
        APTSourceEdit::selectedAptSource = selectedAptSource;
        aptSourceEditDialog->show();
    }
}

void APTSourceManagerPage::on_aptSourceRepositoryListWidget_currentItemChanged(QListWidgetItem *current,
                                                                               QListWidgetItem *previous)
{
    qDebug() << "...";
    if (previous) {
        QWidget *widget = ui->aptSourceRepositoryListWidget->itemWidget(previous);
        if (widget) {
            widget->setStyleSheet("");
        }
    }

    if (current) {
        QWidget *widget = ui->aptSourceRepositoryListWidget->itemWidget(current);
        if (widget) {
            widget->setStyleSheet("background-color: orange;");
        }
    }

//    qApp->setStyleSheet(qApp->styleSheet());
}

























