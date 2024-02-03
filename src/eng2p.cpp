#include "eng2p.h"
#include "eng2p_p.h"
#include <QDebug>

#include "g2pglobal.h"

#if (QT_VERSION >= QT_VERSION_CHECK(6, 0, 0))
#  include <QtCore5Compat>
#endif

namespace IKg2p {
    bool loadArpabetDict(const QString &dict_dir, const QString &fileName,
                         QHash<QString, QString> &resultMap) {
        QString file_path = QDir::cleanPath(dict_dir + "/" + fileName);
        QFile file(file_path);
        if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
            qWarning() << fileName << " error: " << file.errorString();
            return false;
        }
        QRegExp regex("([a-z']+)  ([a-z1-9 ]+)");
        while (!file.atEnd()) {
            QString line = file.readLine().trimmed();
            QStringList spRes = line.split("  ");
            if (regex.indexIn(line) != -1 && spRes.size() == 2) {
                QString key = spRes[0];
                QString values = spRes[1];
                resultMap.insert(key, values);
            }
        }
        return true;
    }

    EnG2pPrivate::EnG2pPrivate() {
    }

    EnG2pPrivate::~EnG2pPrivate() {
    }

    // load arpabet convert dict
    void EnG2pPrivate::init() {
        auto dict_dir = dictionaryPath();
        loadArpabetDict(dict_dir, "cmudict-07b.txt", arpabetMap);
    }

    EnG2p::EnG2p(QObject *parent) : EnG2p(*new EnG2pPrivate(), parent) {
    }

    EnG2p::~EnG2p() {
    }

    QString EnG2p::word2arpabet(const QStringList &wordList, bool removeNum) const {
        Q_D(const EnG2p);
        QStringList arpabetList;
        for (const QString &word : wordList) {
            arpabetList.append(d->arpabetMap.value(word.toLower(), word));
        }

        for (QString &item : arpabetList) {
            if (removeNum) {
                if (item.back().isDigit()) {
                    item.chop(1);
                }
            }
        }

        return arpabetList.join(" ");
    }

    QString EnG2p::word2arpabet(const QString &word, bool removeNum) const {
        Q_D(const EnG2p);
        QStringList input = viewList2strList(splitString(word));
        return word2arpabet(input, removeNum);
    }

    EnG2p::EnG2p(EnG2pPrivate &d, QObject *parent) : QObject(parent), d_ptr(&d) {
        d.q_ptr = this;

        d.init();
    }
}
