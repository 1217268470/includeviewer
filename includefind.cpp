#include <QDebug>
#include <QFile>
#include <QDir>



const QString SRC="";

void changeFile(const QString& path,const  QStringList& hcs){
    QFile readFile(path);
    bool isOK=readFile.open(QIODevice::ReadOnly|QIODevice::Text);
    QStringList lines;
    if(isOK){
        while (!readFile.atEnd()) {
            QByteArray line=readFile.readLine();
            QString ls(line);
            lines.append(ls);
        }
    }
    readFile.close();

    QFile writeFile(path);
    bool wisOK=writeFile.open(QIODevice::ReadWrite|QIODevice::Text);
    if(wisOK){
        QTextStream out(&writeFile);
        for (int i=0, len=lines.size();i<len ;i++ ) {
            QString line=lines.at(i);
            if(line.startsWith("#include <botan")){
                line="//"+line;

                QString suffix=line.mid(line.lastIndexOf("/")+1);
                suffix=suffix.mid(0,suffix.length()-2);
                qDebug()<<suffix;

                for (int j=0,len=hcs.size();j<len ;j++ ) {
                    QString sf=hcs.at(j);
                    QString sff=sf.mid(sf.lastIndexOf("/")+1);
                    if(sff==suffix){
                        qDebug()<<sff;
                        qDebug()<<sf;
                        QString fullsuffix=sf.mid(SRC.length());
                        qDebug()<<fullsuffix;
                        line.append("#include \"").append(fullsuffix).append("\"\n");
                        qDebug()<<line;
                    }

                }

            }
            out<<line;
        }
        writeFile.close();

    }
}

QStringList getFileList(const QString& sPath){
    //    QFileInfoList result;
    QStringList hcs;
    QDir dir(sPath );
    dir.setFilter(QDir::Files | QDir::Hidden | QDir::NoSymLinks | QDir::Dirs | QDir::NoDotAndDotDot);
    dir.setSorting(QDir::Time | QDir::Reversed);
    QFileInfoList fileInfoList = dir.entryInfoList();
    foreach (QFileInfo fileInfo, fileInfoList) {
        if (fileInfo.isDir()) {
            //            result.append(getFileList(fileInfo.filePath()));
            hcs.append(getFileList(fileInfo.filePath()));

        } else {
            if(fileInfo.absoluteFilePath().endsWith(".h")||fileInfo.absoluteFilePath().endsWith(".cpp")){
                //                qDebug()<<fileInfo.absoluteFilePath();
                hcs.append(fileInfo.absoluteFilePath());
                //                result.append(fileInfo);
            }
        }

    }
    return hcs;
}



void change(){
    qDebug("change");
    QStringList files=getFileList(SRC);
    for (int i=0,len=files.size();i<len ;i++ ) {
        QString file=files.at(i);
        changeFile(file,files);

    }
}

void findFile(const QString& fc){
    QStringList hcs=getFileList(SRC);

    for (int i=0, len=hcs.size();i<len ;i++ ) {
        QString hc=hcs.at(i);
        QFile readFile(hc);
        bool isOK=readFile.open(QIODevice::ReadOnly|QIODevice::Text);
        if(isOK){
            while (!readFile.atEnd()) {
                QByteArray line=readFile.readLine();
                QString ls(line);
                if(ls.contains(fc)){
                    qDebug()<<hc;
                }
            }
        }

    }

}
