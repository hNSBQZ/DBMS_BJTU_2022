#include "function.h"
#include <QCoreApplication>
#include<QDebug>
#include <QFile>
#include <QTextStream>
#include <QString>
#include <QSet>
#include<QList>
#include<QMap>
#include<QStack>
#include<QDir>

    QString rootAddress = "C:\\Users\\24625\\Desktop";

    QStringList WriteContent(QStringList tableList)
    {
        QString s=tableList[0];
        QFile tmpFile(rootAddress+"\\"+s+".txt");
        if(!tmpFile.open(QIODevice::ReadOnly|QIODevice::Text)){
                qDebug()<<"文件打开失败";
        }
        QTextStream in(&tmpFile);
        QString str;
        in>>str;
        in>>str;
        in>>str;
        qDebug()<<tableList.size();
        QStringList content;

        if(tableList.size()==1)
        {
            while(!in.atEnd()){
                    in >> str;
                    if(str=="")break;
                    qDebug() << str;
                    content.append(str);
                }
            tmpFile.close();
            return content;
        }
        QStringList content2;
        QStringList tableList_1;
        for(int i=1;i<tableList.size();i++)
            tableList_1.append(tableList[i]);

        content=WriteContent(tableList_1);
        while(!in.atEnd())
        {
            in>>str;
            if(str=="")break;
            foreach(s,content)
            {
                QString newStr=str+"%"+s;
                content2.append(newStr);
            }
        }
        tmpFile.close();
        return content2;

    }

    bool simplyConditionJudge(QString condition,QString data,QMap<QString,int>projection,QMap<QString,QString>dataTypeProjection)
    {
        qDebug()<<"nowtheSimple";

        QStringList dataList=data.split("%",QString::SkipEmptyParts);
         foreach(QString s,dataList)qDebug("%s",qPrintable(s));
        QChar compareOp;
        foreach(QChar c,condition)
        {
            if(c=='='||c=='>'||c=='<')
            {
                compareOp=c;
                break;
            }
        }
        QStringList conditionList=condition.split(QRegExp("[!<>=]"),QString::SkipEmptyParts);
        foreach(QString s,conditionList)qDebug("%s",qPrintable(s));
        int compareInt1,compareInt2,dataType=-1;//dataType 0为int,1为char
        QString compareString1,compareString2;
        bool res;
        qDebug("%s",qPrintable(dataTypeProjection[conditionList[0]]));
        qDebug()<<projection[conditionList[0]];
        qDebug()<<projection[conditionList[1]];
        if(conditionList[0].contains("."))
        {
            if(dataTypeProjection[conditionList[0]].compare("int")==0)
            {
                compareInt1=dataList[projection[conditionList[0]]].toInt();
                dataType=0;
            }
            else
            {
                compareString1=dataList[projection[conditionList[0]]];
                dataType=1;
            }

        }
        if(conditionList[1].contains("."))
        {
            if(dataTypeProjection[conditionList[1]].compare("int")==0)
            {
                compareInt2=dataList[projection[conditionList[1]]].toInt();
                dataType=0;
            }
            else
                compareString2=dataList[projection[conditionList[1]]];
        }
        if(!conditionList[0].contains("."))
        {
            if(dataType==1)
                compareString1=conditionList[0];
            else
                compareInt1=conditionList[0].toInt();
        }
        if(!conditionList[1].contains("."))
        {
            if(dataType==1)
                compareString2=conditionList[1];
            else
                compareInt2=conditionList[1].toInt();
        }

       if(dataType==1)
       {
           if(compareOp=='=')res=(compareString1.compare(compareString2)==0);
           else if(compareOp=='>')res=(compareString1.compare(compareString2)>0);
           else res=(compareString1.compare(compareString2)<0);
       }
       if(dataType==0)
       {
           if(compareOp=='=')res=(compareInt1==compareInt2);
           else if(compareOp=='>')res=(compareInt1>compareInt2);
           else res=(compareInt1<compareInt2);
       }
       qDebug()<<res;
       if(condition[0]=='!')return ~res;
       return res;
    }

    bool JudgeCondition(QString condition,QString data,QMap<QString,int>projection,QMap<QString,QString>dataTypeProjection)
    {
        QStringList conditionList=condition.split(" ",QString::SkipEmptyParts);
        if(condition=="")return true;
        QStack<QString>opr;
        QStack<bool>opn;
        QStringList simplyConditions;
        QString tmpCondition="";
        foreach(QString condition,conditionList)
        {
            qDebug("%s",qPrintable(condition));
            if(condition.compare("and")!=0&&condition.compare("or")!=0)
            {
                if(condition.compare("not")==0)
                    tmpCondition+="!";
                else
                {
                    tmpCondition+=condition;
                    simplyConditions.append(tmpCondition);
                    tmpCondition="";
                }
            }
            else simplyConditions.append(condition);
        }//简化条件语句

       foreach(QString condition,simplyConditions)
        {
            qDebug("%s",qPrintable(condition));
            if(condition.compare("and")==0||condition.compare("or")==0)
                opr.push(condition);
            else
            {
                if(opr.isEmpty())
                    opn.push(simplyConditionJudge(condition,data,projection,dataTypeProjection));
                else
                {
                    bool n1=opn.pop();
                    QString o1=opr.pop();
                    if(o1.compare("and")==0)
                        opn.push(n1&simplyConditionJudge(condition,data,projection,dataTypeProjection));
                    else
                        opn.push(n1|simplyConditionJudge(condition,data,projection,dataTypeProjection));
                }
            }
        }

        return opn.pop();
    }

    int Cmp(QString s1,QString s2,QString key,QMap<QString,int>projection,QMap<QString,QString>dataTypeProjection)
    {
        qDebug()<<"here cmp";
        QStringList List_1=s1.split("%",QString::SkipEmptyParts);
        QStringList List_2=s2.split("%",QString::SkipEmptyParts);
        int rowNum=projection[key];
        qDebug()<<rowNum;
        QString c1=List_1[rowNum];
        QString c2=List_2[rowNum];
        qDebug("%s",qPrintable(c1+"vs"+c2));
        if(dataTypeProjection[key].compare("int")==0)
        {
            int ci1=c1.toInt();
            int ci2=c2.toInt();
            qDebug()<<ci1<<" "<<ci2;
            if(ci1==ci2)return 0;
            else if(ci1>ci2)return 1;
            else return -1;
        }
        if(c1==c2)return 0;
        else if(c1>c2)return 1;
        else return -1;
    }

    QString select(QString attribute,QString table,QString condition,QString order)
    {

        //创建一个中间缓存文件
        QFile file(rootAddress+"\\temp.txt");
        if(!file.open(QIODevice::Append|QIODevice::Text)){
                qDebug()<<"文件打开失败";
        }

        //存储属性的集合
        QSet<QString> attributeSet;
        QStringList attributeList=attribute.split(',',QString::SkipEmptyParts);
        foreach(QString s,attributeList)
        {
            attributeSet.insert(s);
            qDebug("%s",qPrintable(s));
        }

        QMap<QString,int>projection;//投影
        QMap<QString,QString> dataTypeProjection;//数据类型投影

        //根据选中表建立中间文件表头
        qDebug()<<"table";
        qDebug("%s",qPrintable(table));
        QStringList tableList=table.split(',',QString::SkipEmptyParts);
        QString newForm="";//新的头文件表头
        int columnNum=0;
        //QList<QFile> tableFileSet;
        foreach(QString s,tableList)
        {
            qDebug("%s",qPrintable(rootAddress+"\\"+s+".txt"));
            QFile tmpFile(rootAddress+"\\"+s+".txt");
            if(!tmpFile.open(QIODevice::ReadOnly|QIODevice::Text)){
                    qDebug()<<"文件打开失败";
            }
            QTextStream in(&tmpFile);
            QString tmpString;
            in>>tmpString;
            qDebug("%s",qPrintable(tmpString));
            QStringList tmpList=tmpString.split('%',QString::SkipEmptyParts);
            foreach(QString tmps,tmpList)
            {
                qDebug("%s",qPrintable(tmps));
                QStringList attributeTmp=tmps.split('|',QString::SkipEmptyParts);
                projection[s+"."+attributeTmp[2]]=columnNum++;
                dataTypeProjection[s+"."+attributeTmp[2]]=attributeTmp[3];
                QString newString=s+"."+attributeTmp[2]+"|"+attributeTmp[3]+"%";
                newForm+=newString;
            }
            tmpFile.close();
        }
        QTextStream out(&file);
        out<<newForm+"\n";
        QMap<QString,int>::iterator iter=projection.begin();
        while(iter!=projection.end())
        {
            qDebug()<<iter.key()<<":"<<iter.value();
            iter++;
        }

        //把所有数据写进temp文件
        QStringList tableList_1;
        foreach(QString s,tableList)
            tableList_1.append(s);//不确定是值传递还是引用传递
        qDebug()<<"abab";
        tableList_1=WriteContent(tableList);
        foreach(QString s,tableList_1)
            out<<s+"\n";
        file.close();
        //写结果文件

        QFile ResultFile(rootAddress+"\\result.txt");
        if(!ResultFile.open(QIODevice::Append|QIODevice::Text)){
                qDebug()<<"文件打开失败";
        }

        QFile tempFile(rootAddress+"\\"+"temp.txt");
        if(!tempFile.open(QIODevice::ReadOnly|QIODevice::Text)){
                qDebug()<<"文件打开失败";
        }

        QFile temp2File(rootAddress+"\\"+"temp2.txt");
        if(!temp2File.open(QIODevice::Append|QIODevice::Text)){
                qDebug()<<"文件打开失败";
        }

        QTextStream resultOut(&ResultFile);
        QTextStream in(&tempFile);
        QTextStream tempOut(&temp2File);

        QString Header="";

        foreach(QString s,attributeList)
            Header+=s+"%";
        tempOut<<Header<<"\n";

        QString data;
        in>>data;
        qDebug("%s",qPrintable(data));

       // QStringList conditionList=condition.split(" ",QString::SkipEmptyParts);
        int cnt=0;
        while(!in.atEnd())
        {
            in>>data;
            qDebug("%s",qPrintable(data));
            if(data=="")break;
            if(JudgeCondition(condition,data,projection,dataTypeProjection))
                tempOut<<data<<"\n";

            cnt+=1;
            qDebug()<<"shit";
            qDebug()<<cnt;
        }

        //tempFile.remove();

        temp2File.close();
        tempFile.close();


        QFile tempToResult(rootAddress+"\\"+"temp2.txt");
        if(!tempToResult.open(QIODevice::ReadOnly|QIODevice::Text)){
                qDebug()<<"文件打开失败";
        }

        QTextStream toBuffer(&tempToResult);
        toBuffer>>data;
        resultOut<<data<<"\n";

        if(order=="")
        {
            while(!toBuffer.atEnd())
            {
                toBuffer>>data;
                qDebug("%s",qPrintable(data));
                if(data=="")break;
                QStringList dataList=data.split("%",QString::SkipEmptyParts);
                QString rowData;
                foreach(QString s,attributeList)
                    rowData+=dataList[projection[s]]+"%";
                resultOut<<rowData<<"\n";
            }
        }
        else
        {
            QStringList orderList=order.split(",",QString::SkipEmptyParts);
            QStringList oneOforder=orderList[0].split(" ",QString::SkipEmptyParts);
            QString key=oneOforder[0];
            QStringList resultInOrder;
            QString direct=oneOforder[1];
            qDebug()<<"order!";

            while(!toBuffer.atEnd())
            {
                bool haveInserted=false;
                int place=0;
                toBuffer>>data;
                if(data=="")break;
                qDebug("%s",qPrintable(data));
                if(resultInOrder.isEmpty())resultInOrder.append(data);
                else
                {
                    foreach(QString s,resultInOrder)
                    {
                        if(direct[0]=='D')
                        {
                            int CmpNum=Cmp(data,s,key,projection,dataTypeProjection);
                            qDebug()<<"the cmpNum";
                            qDebug()<<CmpNum;
                            if(CmpNum>0)
                            {
                                qDebug()<<place;
                                qDebug("%s",qPrintable("inserted"+data));
                                resultInOrder.insert(place,data);
                                haveInserted=true;
                                break;
                            }
                            place+=1;
                        }
                        else
                        {

                            if(Cmp(data,s,key,projection,dataTypeProjection)<0)
                            {
                                resultInOrder.insert(place,data);
                                haveInserted=true;
                                break;
                            }
                            place+=1;
                        }
                    }
                    if(!haveInserted)resultInOrder.append(data);
                }

            }

            foreach(QString s,resultInOrder)
            {
                    QStringList dataList=s.split("%",QString::SkipEmptyParts);
                    QString rowData;
                    if(rowData == "") break;
                    foreach(QString s1,attributeList)
                        rowData+=dataList[projection[s1]]+"%";
                    resultOut<<rowData<<"\n";
            }
        }
        tempToResult.close();
        tempToResult.remove();
        tempFile.remove();
        ResultFile.close();

        return rootAddress+"\\result.txt";

    }

    QString rewriteFile(QString file_addr,QString target_tableName){
        QFile resultFile(file_addr);
        if(!resultFile.open(QIODevice::ReadOnly|QIODevice::Text)){
                qDebug()<<"文件打开失败";
        }

        QFile targetFile(rootAddress+"\\"+target_tableName+".txt");
        if(!targetFile.open(QIODevice::WriteOnly|QIODevice::Text)){
                qDebug()<<"文件打开失败";
        }

        QTextStream result_in(&resultFile);
        QTextStream result_out(&targetFile);

        QString data;

        while(!result_in.atEnd()){
            result_in >> data;
            if(data=="")break;
            qDebug("%s",qPrintable(data));
            result_out << data << "\n";
        }

        resultFile.close();
        targetFile.close();

        return rootAddress + "\\" + target_tableName + ".txt";
    }

    QString delete_function(QString table,QString condition)
    {
        QString decision;

        decision = referenceConstraints(table);
        if(decision == "yes"){
            return  "DeleteWrong";
        }
        //创建一个中间缓存文件
        QFile file(rootAddress+"\\temp.txt");
        if(!file.open(QIODevice::Append|QIODevice::Text)){
                qDebug()<<"文件打开失败";
        }

        QMap<QString,int>projection;//投影
        QMap<QString,QString> dataTypeProjection;//数据类型投影

        //根据选中表建立中间文件表头
        qDebug()<<"table";
        qDebug("%s",qPrintable(table));
        QStringList tableList=table.split(',',QString::SkipEmptyParts);
        QString newForm="";//新的头文件表头
        int columnNum=0;
        //QList<QFile> tableFileSet;
        foreach(QString s,tableList)
        {
            qDebug("%s",qPrintable(rootAddress+"\\"+s+".txt"));
            QFile tmpFile(rootAddress+"\\"+s+".txt");
            if(!tmpFile.open(QIODevice::ReadOnly|QIODevice::Text)){
                    qDebug()<<"文件打开失败";
            }
//读取from中的文件tempFile记录信息
            QTextStream in(&tmpFile);
            QString tmpString;
            in>>tmpString;
            qDebug("%s",qPrintable(tmpString));
//1|0|sno|char8%0|0|sname|char10%0|0|sex|char1%0|1|classno|char8%0|0|totalCredit|int
//分割每个属性信息
//projrction存属性下标，dataTypeProjection存属性类型——key,value
            QStringList tmpList=tmpString.split('%',QString::SkipEmptyParts);
            foreach(QString tmps,tmpList)
            {
                qDebug("%s",qPrintable(tmps));
                QStringList attributeTmp=tmps.split('|',QString::SkipEmptyParts);
                projection[s+"."+attributeTmp[2]]=columnNum++;
                dataTypeProjection[s+"."+attributeTmp[2]]=attributeTmp[3];
                QString newString=s+"."+attributeTmp[2]+"|"+attributeTmp[3]+"%";
                newForm+=newString;
            }
            tmpFile.close();
        }
//输出信息至file文件
        QTextStream out(&file);
        out<<newForm+"\n";
        QMap<QString,int>::iterator iter=projection.begin();
        while(iter!=projection.end())
        {
            qDebug()<<iter.key()<<":"<<iter.value();
            iter++;
        }

        //把所有数据写进file文件
        QStringList tableList_1;
        foreach(QString s,tableList)
            tableList_1.append(s);//不确定是值传递还是引用传递
        qDebug()<<"abab";
        tableList_1=WriteContent(tableList);
        foreach(QString s,tableList_1)
            out<<s+"\n";
        file.close();
        //写结果文件

        QFile ResultFile(rootAddress+"\\result.txt");
        if(!ResultFile.open(QIODevice::Append|QIODevice::Text)){
                qDebug()<<"文件打开失败";
        }

        QFile tempFile(rootAddress+"\\"+"temp.txt");
        if(!tempFile.open(QIODevice::ReadOnly|QIODevice::Text)){
                qDebug()<<"文件打开失败";
        }

        QFile temp2File(rootAddress+"\\"+"temp2.txt");
        if(!temp2File.open(QIODevice::Append|QIODevice::Text)){
                qDebug()<<"文件打开失败";
        }

        QFile targetFile(rootAddress+"\\"+table+".txt");
        if(!targetFile.open(QIODevice::ReadOnly|QIODevice::Text)){
                qDebug()<<"文件打开失败";
        }

        QTextStream resultOut(&ResultFile);
        QTextStream in(&tempFile);
        QTextStream tempOut(&temp2File);
        QTextStream character_in(&targetFile);

        QString Header="";

        QString data;
        in>>data;
        qDebug("%s",qPrintable(data));

        //QStringList conditionList=condition.split(" ",QString::SkipEmptyParts);
        int cnt=0;
        while(!in.atEnd())
        {
            in>>data;
            qDebug("%s",qPrintable(data));
            if(data=="")break;
            if(JudgeCondition(condition,data,projection,dataTypeProjection))
                tempOut<<data<<"\n";

            cnt+=1;
            qDebug()<<"shit";
            qDebug()<<cnt;
        }

        //tempFile.remove();

        temp2File.close();
        tempFile.close();


        QFile tempToResult(rootAddress+"\\"+"temp2.txt");
        if(!tempToResult.open(QIODevice::ReadOnly|QIODevice::Text)){
                qDebug()<<"文件打开失败";
        }

        QTextStream toBuffer(&tempToResult);
        QStringList delete_row;

        toBuffer>>data;
        delete_row.append(data);
//        resultOut<<data<<"\n";

            while(!toBuffer.atEnd())
            {
                toBuffer>>data;
                qDebug("%s",qPrintable(data));
                delete_row.append(data);
//                resultOut<<data<<"\n";
                if(data=="")break;
                QStringList dataList=data.split("%",QString::SkipEmptyParts);
            }

            if(delete_row.size() == 0){
                qDebug() << "没有匹配数据";
                return "DeleteWrong";
            }else{
                foreach(QString s , delete_row){
                    qDebug("%s",qPrintable(s));
                }
            }

            while(!character_in.atEnd()){
                character_in >> data;
                if(data == "") break;
                    if(!delete_row.contains(data)){
                        resultOut << data << "\n";
                    }
            }


        targetFile.close();
        tempToResult.close();
        tempToResult.remove();
        tempFile.remove();
        ResultFile.close();

        rewriteFile(rootAddress+"\\result.txt",table);

        return "DeleteOk";
    }

    QString primarykey(QMap<QString,int> projection,QString table,QString file_addr){
        QFile targetFile(rootAddress + "\\" + table + ".txt");
        if(!targetFile.open(QIODevice::ReadOnly|QIODevice::Text)){
                qDebug()<<"文件打开失败";
        }

        QTextStream out(&targetFile);

        int index = 0;
        QString data;
        QStringList primaryKey;
        QStringList projection_index;
        QStringList temp;
        QStringList primary_content;
        QMap<QString,int> :: Iterator iter;

        out >> data;
        out >> data;

        if(!QString::compare(QString(data),QString("NF"))){
            targetFile.close();
            return "no";
        }
        else{
            primaryKey = data.split("%");
            for(int i = 0 ; i < primaryKey.size() ; i++){
                primaryKey[i] = table + "." + primaryKey[i];
            }
            foreach(QString s,primaryKey){
                qDebug() << "primaryKey";
                qDebug("%s",qPrintable(s));
            }

            foreach(QString s , primaryKey){
                iter = projection.find(s);
                projection_index.append(iter.key());
                qDebug() << iter.key()  << " " << iter.value();
            }

            QFile resultFile(file_addr);
            if(!resultFile.open(QIODevice::ReadOnly|QIODevice::Text)){
                    qDebug()<<"文件打开失败";
            }

            QTextStream out_result(&resultFile);
            QString temp_data;

            out_result >> data;
            out_result >> data;
            out_result >> data;

            qDebug("%s",qPrintable(data));

            while (!out_result.atEnd()) {
                temp_data = "";
                out_result >> data;
                temp = data.split("%");
                for(int i = 0 ; i < projection_index.size() ; i++){
                    temp_data = temp_data + temp[projection_index[i].toInt()];
                }
                qDebug("%s",qPrintable(temp_data));
                primary_content.append(temp_data);
                index++;
            }

            for(int i = 0 ; i < primary_content.size() ; i++){
                for(int j = 0 ; j < primary_content.size() ; j++){
                    if(primary_content[i] == primary_content[j] && i != j){
                        return "yes";
                    }
                }
            }

            return "no";
        }
    }

    QString foreignkey(QStringList set_KeyValue,QString table){
        QFile targetFile(rootAddress + "\\" + table + ".txt");
        if(!targetFile.open(QIODevice::ReadOnly|QIODevice::Text)){
                qDebug()<<"文件打开失败";
        }

        QTextStream out(&targetFile);

        QString data;
        QStringList temp_foreignkey;
        QStringList foreignkey;

        out >> data;
        out >> data;
        out >> data;

        if(!QString::compare(QString(data),QString("NF"),Qt::CaseSensitive)){
            targetFile.close();
            return "no";
        }

        temp_foreignkey = data.split("%");
        for(int i = 0 ; i < temp_foreignkey.size() ; i++){
            temp_foreignkey[i] = table + "." + temp_foreignkey[i];
        }

        foreach(QString s , temp_foreignkey){
            qDebug("%s",qPrintable(s));
            foreignkey.append(s.split("|"));
        }


        if(!foreignkey.contains(set_KeyValue[0])){
           qDebug("%s",qPrintable(data));
           qDebug("%s",qPrintable(set_KeyValue[0]));
           qDebug() << "1";
           targetFile.close();
           return "no";
        }
        else{
            for(int i = 0 ; i < foreignkey.size() ; i++){

                if(!QString::compare(QString(foreignkey[i]),QString(set_KeyValue[0]),Qt::CaseSensitive)){
                    QString foreign_table;
                    QString foreign_column;

                   foreign_table = foreignkey[i+1];
                   foreign_column = foreignkey[i+1] + "." + foreignkey[i+2];

                   select("",foreign_table,foreign_column + "=" + set_KeyValue[1],"");
                   QFile ResultFile(rootAddress+"\\result.txt");
                   if(!ResultFile.open(QIODevice::ReadOnly|QIODevice::Text)){
                      qDebug()<<"文件打开失败";
                    }

                        QTextStream out(&ResultFile);
                        QString data;

                        out >> data;
                        if(data.isNull()){
                            qDebug() << "2";
                            qDebug("%s",qPrintable(data));
                            qDebug() << "外键值不存在";
                            targetFile.close();
                            ResultFile.remove();
                            return "yes";
                        }
                        else{
                            qDebug("%s",qPrintable(data));
                            targetFile.close();
                            ResultFile.remove();
                            return "no";
                        }
                    }
                }
            }
    }

    QString update_function(QString table,QString set,QString condition)
    {
        QStringList set_KeyValue;

        set = set.simplified();
        set_KeyValue = set.split("=");

        QString decision = "null";

            decision = foreignkey(set_KeyValue,table);
            if(decision == "no"){
                //创建一个中间缓存文件
                QFile file(rootAddress+"\\temp.txt");
                if(!file.open(QIODevice::Append|QIODevice::Text)){
                        qDebug()<<"文件打开失败";
                }

                QMap<QString,int>projection;//投影
                QMap<QString,QString> dataTypeProjection;//数据类型投影

                //根据选中表建立中间文件表头
                qDebug()<<"table";
                qDebug("%s",qPrintable(table));
                QStringList tableList=table.split(',',QString::SkipEmptyParts);
                QString newForm="";//新的头文件表头
                int columnNum=0;
                //QList<QFile> tableFileSet;
                foreach(QString s,tableList)
                {
                    qDebug("%s",qPrintable(rootAddress+"\\"+s+".txt"));
                    QFile tmpFile(rootAddress+"\\"+s+".txt");
                    if(!tmpFile.open(QIODevice::ReadOnly|QIODevice::Text)){
                            qDebug()<<"文件打开失败";
                    }
        //读取from中的文件tempFile记录信息
                    QTextStream in(&tmpFile);
                    QString tmpString;
                    in>>tmpString;
                    qDebug("%s",qPrintable(tmpString));
        //1|0|sno|char8%0|0|sname|char10%0|0|sex|char1%0|1|classno|char8%0|0|totalCredit|int
        //分割每个属性信息
        //projrction存属性下标，dataTypeProjection存属性类型——key,value
                    QStringList tmpList=tmpString.split('%',QString::SkipEmptyParts);
                    foreach(QString tmps,tmpList)
                    {
                        qDebug("%s",qPrintable(tmps));
                        QStringList attributeTmp=tmps.split('|',QString::SkipEmptyParts);
                        projection[s+"."+attributeTmp[2]]=columnNum++;
                        dataTypeProjection[s+"."+attributeTmp[2]]=attributeTmp[3];
                        QString newString=s+"."+attributeTmp[2]+"|"+attributeTmp[3]+"%";
                        newForm+=newString;
                    }
                    tmpFile.close();
                }
        //输出信息至file文件
                QTextStream out(&file);
                out<<newForm+"\n";
                QMap<QString,int>::iterator iter=projection.begin();
                while(iter!=projection.end())
                {
                    qDebug()<<iter.key()<<":"<<iter.value();
                    iter++;
                }

                //把所有数据写进file文件
                QStringList tableList_1;
                foreach(QString s,tableList)
                    tableList_1.append(s);//不确定是值传递还是引用传递
                qDebug()<<"abab";
                tableList_1=WriteContent(tableList);
                foreach(QString s,tableList_1)
                    out<<s+"\n";
                file.close();
                //写结果文件

                QFile ResultFile(rootAddress+"\\result.txt");
                if(!ResultFile.open(QIODevice::Append|QIODevice::Text)){
                        qDebug()<<"文件打开失败";
                }

                QFile tempFile(rootAddress+"\\"+"temp.txt");
                if(!tempFile.open(QIODevice::ReadOnly|QIODevice::Text)){
                        qDebug()<<"文件打开失败";
                }

                QFile temp2File(rootAddress+"\\"+"temp2.txt");
                if(!temp2File.open(QIODevice::Append|QIODevice::Text)){
                        qDebug()<<"文件打开失败";
                }

                QFile targetFile(rootAddress+"\\"+table+".txt");
                if(!targetFile.open(QIODevice::ReadOnly|QIODevice::Text)){
                        qDebug()<<"文件打开失败";
                }

                QTextStream resultOut(&ResultFile);
                QTextStream in(&tempFile);
                QTextStream tempOut(&temp2File);
                QTextStream character_in(&targetFile);

                QString Header="";

                QString data;
                in>>data;
                qDebug("%s",qPrintable(data));

                //QStringList conditionList=condition.split(" ",QString::SkipEmptyParts);
                int cnt=0;
                while(!in.atEnd())
                {
                    in>>data;
                    qDebug("%s",qPrintable(data));
                    if(data=="")break;
                    if(JudgeCondition(condition,data,projection,dataTypeProjection))
                        tempOut<<data<<"\n";

                    cnt+=1;
                    qDebug()<<"shit";
                    qDebug()<<cnt;
                }

                //tempFile.remove();

                temp2File.close();
                tempFile.close();


                QFile tempToResult(rootAddress+"\\"+"temp2.txt");
                if(!tempToResult.open(QIODevice::ReadOnly|QIODevice::Text)){
                        qDebug()<<"文件打开失败";
                }

                QTextStream toBuffer(&tempToResult);
                QStringList update_row;
                QStringList update_content;
                QMap<QString,int>::iterator it=projection.begin();

                toBuffer>>data;
                update_row.append(data);
        //        resultOut<<data<<"\n";

                    while(!toBuffer.atEnd())
                    {
                        toBuffer>>data;
                        qDebug("%s",qPrintable(data));
                        update_row.append(data);
        //                resultOut<<data<<"\n";
                        if(data=="")break;
                        QStringList dataList=data.split("%",QString::SkipEmptyParts);
                    }

                    if(update_row.size() == 0){
                        qDebug() << "没有匹配数据";
                        return "UpdateWrong";
                    }else{
                        foreach(QString s , update_row){
                            qDebug() << "更新行";
                            qDebug("%s",qPrintable(s));
                        }
                    }

                    while(!character_in.atEnd()){
                        character_in >> data;
                        if(data=="")break;
                            if(update_row.contains(data)){
                                QString new_data;
                                qDebug("%s",qPrintable(set_KeyValue[0]));
                                update_content = data.split("%");
                                it = projection.find(set_KeyValue[0]);
                                update_content[it.value()] = set_KeyValue[1];
                                foreach(QString s, update_content){
                                new_data = new_data + s + "%";
                                }
                                new_data = new_data.left(new_data.size() - 1);
                                resultOut << new_data << "\n";
                            }else{
                                 resultOut << data << "\n";
                            }
                 }


                targetFile.close();
                tempToResult.close();
                tempToResult.remove();
                tempFile.remove();
                ResultFile.close();


                decision = primarykey(projection,table,rootAddress+"\\result.txt");
                qDebug("%s",qPrintable(decision));

                if(decision == "yes"){
                    qDebug() << "主键应非空唯一";
                    return "UpdateWrong";
                }
                else{
                    rewriteFile(rootAddress+"\\result.txt",table);
                    return "UpdateOk";
                }

            }
            else {
                return "UpdateWrong";
            }
    }

    QString referenceConstraints(QString table)
    {
        QFile relationFile(rootAddress + "\\relation.txt");
        if(!relationFile.open(QIODevice::ReadOnly|QIODevice::Text)){
                qDebug()<<"文件打开失败";
        }

        QTextStream relation_in(&relationFile);
        QString data;
        QStringList temp_relation;
        QStringList relation;
        QStringList temp_relationTable;
        QStringList relationTable;

        while(!relation_in.atEnd()){
            relation_in >> data;
            if(data=="") break;
            temp_relation = data.split("-");
            relation.append(temp_relation[0]);
        }

        if(relation.size() == 0){
            return "no";
        }
        else{
            foreach(QString s, relation){
                temp_relationTable = s.split(".");
                relationTable.append(temp_relationTable[0]);
                qDebug() << temp_relationTable[0];
            }

            if(relationTable.contains(table)){
                return "yes";
            }
            else {
                return "no";
            }
        }
    }

    QString insert(QString tableName,QString value)
    {
        QStringList valueList=value.split(",",QString::SkipEmptyParts);
        QMap<QString,int>projection;
        QString tableForm;
        QFile readFile(rootAddress+"\\"+tableName+".txt");
        if(!readFile.open(QIODevice::ReadOnly|QIODevice::Text))
            qDebug()<<"打开文件失败";
        //qDebug("%s",qPrintable(rootAddress+tableName+".txt"));
        QTextStream in(&readFile);
        in>>tableForm;
        qDebug("%s",qPrintable(tableForm));
        QStringList tableFormList=tableForm.split("%",QString::SkipEmptyParts);
        int cnt=0;

        foreach(QString s,tableFormList)
        {
            QStringList attributeList=s.split("|",QString::SkipEmptyParts);
            projection[attributeList[2]]=cnt++;
            qDebug("%s",qPrintable(attributeList[2]));
            qDebug()<<cnt-1;
        }

        QString primaryKey;
        in>>primaryKey;
        qDebug("%s",qPrintable(primaryKey));
        QStringList primaryKeyList=primaryKey.split("%",QString::SkipEmptyParts);

        QString ForignKey;
        in>>ForignKey;
        qDebug("%s",qPrintable(ForignKey));

        QString data;
        QSet<QString>primaryKeySet;

        while(!in.atEnd())
        {
            qDebug()<<"the set";
            in>>data;
            if(data=="")break;
            qDebug("%s",qPrintable(data));
            QStringList dataList=data.split("%",QString::SkipEmptyParts);
            QString PK="";
            foreach(QString s,primaryKeyList)
                PK+=dataList[projection[s]]+"%";
            qDebug("%s",qPrintable(PK));
            primaryKeySet.insert(PK);
        }
        readFile.close();

        QString PK="";
        qDebug()<<"??";
        foreach(QString s,primaryKeyList)
            PK+=valueList[projection[s]]+"%";
        qDebug("%s",qPrintable(PK));
        if(primaryKeySet.contains(PK))
            return "InsertWrong";

        QStringList ForignKeyList=ForignKey.split("%",QString::SkipEmptyParts);

        foreach(QString s,ForignKeyList)
        {
            QSet<QString>FKSet;
            QStringList referenceData=s.split("|",QString::SkipEmptyParts);
            QFile tmpFile(rootAddress+"\\"+referenceData[1]+".txt");
            if(!tmpFile.open(QIODevice::ReadOnly|QIODevice::Text))
                qDebug()<<"打开文件失败";
            QTextStream tmpIn(&tmpFile);

            QString dataIntmp;
            tmpIn>>dataIntmp;
            QStringList tmpFormList=dataIntmp.split("%",QString::SkipEmptyParts);
            QMap<QString,int>tmpProjection;
            int tmpCnt=0;
            foreach(QString s1,tmpFormList)
            {
                QStringList TmpAttributeList=s1.split("|",QString::SkipEmptyParts);
                tmpProjection[TmpAttributeList[2]]=tmpCnt++;
                qDebug("%s",qPrintable(TmpAttributeList[2]));
                qDebug()<<tmpCnt-1;
            }
            tmpIn>>dataIntmp;
            tmpIn>>dataIntmp;

            while(!tmpIn.atEnd())
            {
                tmpIn>>dataIntmp;
                if(dataIntmp=="")break;
                 qDebug("%s",qPrintable(dataIntmp));
                QStringList dataIntmpList=dataIntmp.split("%",QString::SkipEmptyParts);
                FKSet.insert(dataIntmpList[tmpProjection[referenceData[2]]]);
            }
            tmpFile.close();
            qDebug()<<"isHere";
            qDebug("%s",qPrintable(valueList[projection[referenceData[0]]]));
            if(!FKSet.contains(valueList[projection[referenceData[0]]]))
                return "InsertWrong";
        }
        qDebug()<<"wtf!!";

        QFile writeFile(rootAddress+"\\"+tableName+".txt");
        if(!writeFile.open(QIODevice::Append|QIODevice::Text))
            qDebug()<<"打开文件失败";
        QTextStream out(&writeFile);
        //out<<"\n";
        QString outString="";
        foreach(QString s,valueList)
            outString+=s+"%";
        out<<outString.left(outString.size()-1)+"\n";
        writeFile.close();
        return "InsertOk";

    }

    QString AlterTable(QString operate,QString tableName,QString columnname,QString Datatype)
    {

        QString tableForm;
        qDebug("%s",qPrintable(rootAddress+"\\"+tableName+".txt"));
        QFile readFile(rootAddress+"\\"+tableName+".txt");
        if(!readFile.open(QIODevice::ReadOnly|QIODevice::Text))
        {return "打开文件失败";}
        QTextStream in(&readFile);
        in>>tableForm;
        QString line2,line,line3;
        in>>line2;
        QStringList tableFormList=tableForm.split("%",QString::SkipEmptyParts);
        QSet<QString> attributeSet;
        foreach(QString s,tableFormList)
        {
            QStringList tempList=s.split("|",QString::SkipEmptyParts);
            attributeSet.insert(tempList[2]);
        }
        in>>line3;
        if(operate=="ADD")
            {
                if(tableFormList.contains(columnname)==true){return "ADDWrong";}
                else
            {
                    tableForm+="%0|0|";
                    tableForm+=columnname;
                    tableForm+="|";
                    tableForm+=Datatype;
                QFile writeFile(rootAddress+"\\"+tableName+"1.txt");
                if(!writeFile.open(QIODevice::Append|QIODevice::Text))
                        return"打开文件失败";
                 QTextStream out(&writeFile);
                out << tableForm << endl;
                out << line2 << endl;
                out<<line3<<"\n";
                while(!in.atEnd())
                   {
                    in>>line;
                    if(line=="")break;
                    qDebug("%s",qPrintable(line));
                    line+="%NULL";
                       out << line<< endl;
                   }
                 writeFile.close();
                 readFile.close();
                 readFile.remove();
                 writeFile.rename(rootAddress+"\\"+tableName+".txt");
                 return "ADDOK";
        }
            }

        if(operate=="DROP"){
            QStringList primeKeyList=line2.split("%",QString::SkipEmptyParts);
            if(primeKeyList.contains(columnname)==true){return "DropWrong";}
            if(attributeSet.contains(columnname)!=true){return "DropWrong";}

            QString FKReference=tableName+"."+columnname;
            QFile relationFile(rootAddress+"\\relation.txt");
            if(!relationFile.open(QIODevice::ReadOnly|QIODevice::Text))
                    return"打开文件失败";
            QTextStream relationIn(&relationFile);
            while(!relationIn.atEnd())
            {
                relationIn>>line;
                QStringList tempList=line.split("-");
                if(tempList[0]==FKReference)
                {
                    relationFile.close();
                    return "DropWrong";
                }
            }
            relationFile.close();

            int dropNum=0,cnt=0;
            QString line1="";
            foreach(QString s,tableFormList)
            {
                QStringList tempList=s.split("|",QString::SkipEmptyParts);
                cnt+=1;
                if(tempList[2]==columnname){dropNum=cnt;continue;}
                else {line1+=s+"%";}
            }
            QFile writeFile(rootAddress+"\\"+tableName+"1.txt");
            if(!writeFile.open(QIODevice::Append|QIODevice::Text))
                    return"打开文件失败";
             QTextStream out(&writeFile);

            out<<line1<<"\n";
            out << line2 << endl;
            out<<line3<<"\n";

             QStringList valueList;
             while(in.atEnd() == false)
                {
                 in>>line;
                 valueList=line.split("%",QString::SkipEmptyParts);
                 valueList.removeAt(dropNum-1);
                 out << valueList.join("%")<< endl;
                }

        writeFile.close();
        readFile.close();
        readFile.remove();
        writeFile.rename(rootAddress+"\\"+tableName+".txt");
        return "DROPOK";

    }}

    QString CreateUsers(QString userName,QString password)
    {
       QString k;
       QString filename = rootAddress+"\\user.txt";  //以只读方式打开文件
       QFile file(filename);
       file.open(QIODevice::ReadOnly| QIODevice::Text);
       QString a=file.readAll();            //从文件中读取数据
       QStringList strlist = a.split("\n"); //将读取的数据通过转行符进行分割
       file.close();                        //关闭文件
       QFile file2(filename);               //以只写方式打开文件
       file2.open(QIODevice::WriteOnly| QIODevice::Text);
       QTextStream wstream(&file2);         //创建文件输入流
       if(a.isEmpty()){                     //文件里没有信息，就直接插入数据
           wstream<<userName<<'|'<<password;
       }
       else{                               //文件里有信息，将分割出来的信息再通过 | 进行分割
           for(int i=0;i<strlist.size();i++){

               QString b=strlist.at(i);
               QStringList strlist2 = b.split("|");
               if(strlist2.at(0) == userName){  //判断哪些与所传的userName变量相同，将相同的数据删除
                   strlist.removeAt(i);

               }
           }

    file2.resize(0);   //将文件内容清空
    for(int m=0;m<strlist.size();m++){ //重新写入文件
        wstream<<strlist.at(m)<<endl;
    }

     wstream<<userName<<'|'<<password;//向文件添加传递到函数的变量所产生的用户信息


       }
       k="CreateOk";
       return k;
    }

    QString OperateRights(QString operate,QString userName,QString tableName,QString right){

        QString opp;
       if(operate.toLower() == "grant"){   //如果是授权
           QFile file (rootAddress+"\\right.txt");  //以读写方式打开文件
           file.open(QIODevice::ReadWrite| QIODevice::Text);
            QTextStream wstream(&file);   //创建数据输入流
            QByteArray array=file.readAll();
            QString k;               //判断授予的权限是否在五个权限之中
            qint8 l=0;                  //同上
            QStringList strlist = right.split(",");//以逗号分割信息
            for(int i = 0;i<strlist.size();i++){
              k=strlist.at(i);                    //提取分割的信息，循环判断以支持同时授权或回收多个权限。
              if(k!="select" && k!="alter" && k!="update" && k!="delete" && k!="insert" ){//如果不是规定五个权限之中的，报错
                  l=1;
                  opp="Wrong in Right";
              }
            }
            if(l == 0){                              //如果在五个权限之中，循环将信息写入文件，并返回信息
                 for(int m=0;m<strlist.count();m++){

                wstream<<userName<<'%'<<tableName<<'%'<<strlist.at(m)<<'\n';
             }
               opp= "GrantOK";

            }
                 file.close();

       }
       else if(operate.toLower() == "revoke"){ //如果是回收权限
           QFile file (rootAddress+"\\right.txt");
           file.open(QIODevice::ReadOnly| QIODevice::Text); //以只读方式打开文件
           qint8 h = 0; //用来判断是否出错
           QString strall=file.readAll();
            QStringList strlist2=strall.split("\n");//通过转行符将文件信息分割
           QStringList strlist = right.split(",");//通过逗号将要回收信息的权限分割
           file.close();
               for(int m=0;m<strlist2.size();m++){ //循环查看文件中是否有对应用户，对应表，对应要回收的权限的信息
                   for(int n=0;n<strlist.size();n++){
                       if(strlist2.at(m) == userName+"%"+tableName+"%"+strlist.at(n)){
                        strlist2.removeAt(m); //有就将提取出的信息删除
                        h=1;
                       }
                   }


           }
               if(h==0){         //没有就报错
                  opp="RevokeFail";
               }
               else{   //有就先显示回收权限成功
                   opp="RevokeOK";
               }
               QFile file2(rootAddress+"\\right.txt"); //以只写方式打开文件
               file2.open(QIODevice::WriteOnly| QIODevice::Text);
               QTextStream edit(&file2);
               file2.resize(0); //清空文件内容
               for(int i=0;i<strlist2.size();i++){ //重写信息到文件中
                   edit<<strlist2.at(i)<<endl;
               }

               file2.close(); //关闭文件
       }
       else{
               //不是授权也不是回收权限就报错
                opp="Authorization erroes";

       }

       return opp;
    }

    QString CreateTables(QString tableName,QString content){
        QString k;
        QString fileName = rootAddress+"\\"+tableName+".txt";//创建表
        QFile file(fileName);
        QString fileName2 = rootAddress+"\\relation.txt";//创建relation表
         QFile fileR(fileName2);
         fileR.open(QIODevice::ReadWrite| QIODevice::Text);
             QByteArray array=fileR.readAll();
        if(file.exists()){   //表存在就报错
         k="TableCreateWrong";
        }
        else{
        bool openok=file.open(QIODevice::ReadWrite| QIODevice::Text);
        if(openok ){
        QTextStream wstream(&file);
         QTextStream wstream2(&fileR);
        qint8 p=0;  //对应是否为主键的数字
        qint8 u=0;   //对应是否为外键的数字
        int w=0;
        int f=0;
        QString h="PK";
         QString z="FK";
        QStringList strlist = content.split(",\n");//以逗号加转行符作为分割

        for(int m =0;m<strlist.size();m++){
            if(strlist.at(m).contains(h,Qt::CaseInsensitive)){//判断每一行是否包含不区分大小写的pk，是的话p为1，否则为0
               p=1;
            }
            else{
                p=0;
            }
            if(strlist.at(m).contains(z,Qt::CaseInsensitive)){//判断每一行是否包含不区分大小写的fk，是的话u为1，否则为0
               u=1;
            }
            else{
               u=0;
            }
            QString ac = strlist.at(m);
            QStringList strlist1 = ac.split(" ");//以空格为区分进一步分割信息
             if(strlist1.at(0).compare(h,Qt::CaseInsensitive) && strlist1.at(0).compare(z,Qt::CaseInsensitive)){//如果第一块不是PK或者NK（我也不知道为啥不用！）
                 wstream<<p<<'|'<<u; //将信息填入进去
            for(int n =0;n<strlist1.size();n++){
                if(n!=strlist1.size()-1 ){ //如果此块儿信息不是一行得最后一块儿信息
                    if(!strlist1.at(n+1).compare(h,Qt::CaseInsensitive) ||!strlist1.at(n+1).compare(z,Qt::CaseInsensitive)){//不同方式写入
                            wstream<<'|'<<strlist1.at(n)<<'%';
                   }
                    else{
                    wstream<<'|'<<strlist1.at(n);
                    }
                }
                if(n ==strlist1.size()-1){//如果此块儿信息是一行得最后一块儿信息
                    if(!strlist1.at(n).compare(h,Qt::CaseInsensitive) ||!strlist1.at(n).compare(z,Qt::CaseInsensitive)){
                       continue; //如果是pk或者fk就略过
                   }
                    else{
                        wstream<<'|'<<strlist1.at(n)<<'%';//否则写入
                    }

                }

            }

        }
             else if(!strlist1.at(0).compare(h,Qt::CaseInsensitive)){//如果这一行的第一块儿信息是PK
                 w=1;
                 wstream<<'\n';
                  for(int n = 1;n<strlist1.size();n++){//不把PK写入，不同格式写入
                      if(n!=strlist1.size()-1){
                      wstream<<strlist1.at(n)<<'%';
                      }
                      else{
                          wstream<<strlist1.at(n);
                      }
                  }
             }

             else if(!strlist1.at(0).compare(z,Qt::CaseInsensitive)){//如果这一行的第一块儿信息是FK
                     f=1;

                     wstream<<'\n';
                     if(w==0){
                         QString sdd= "NP";
                         wstream<<sdd<<'\n';
                     }
                     for(int n = 1;n<strlist1.size();n++){ //不把FK写入，不同格式写入
                         if(n!=strlist1.size()-1){
                         wstream<<strlist1.at(n)<<'|';
                         }
                         else{
                             wstream<<strlist1.at(n)<<'%';
                         }
                     }

                          int k = strlist1.size()-2;        //写入到relation文件中
                          wstream2<<strlist1.at(k)<<'.';
                           k = strlist1.size()-1;
                           wstream2<<strlist1.at(k)<<'-';
                           wstream2<<tableName<<'.';
                           k = strlist1.size()-3;
                           wstream2<<strlist1.at(k)<<'\n';

             }


        }
        if(w==0 && f==0){ //没有主键和外键，第二行添NP
             wstream<<'\n';
            QString sdd= "NP";
            wstream<<sdd<<'\n';
            sdd= "FP";
            wstream<<sdd<<'\n';
        }

        if(w==1 && f==0){
             QString sdd= "FP";
            wstream<<sdd<<'\n';
        }
      k="CreateOK";
        }
        }
        file.close();
        return k;
    }

    QString DropTables(QString tableName){

        QString krr;
        int h=0;
    QStringList strlist3 = tableName.split(",");
    for(int k=0;k<strlist3.size();k++){ //循环尝试打开创建好的表
        QString fileName = rootAddress+"\\"+strlist3.at(k)+".txt";
        QFile file(fileName);
        if(!file.exists()){ //打不开就报错
            qDebug()<<"DropTableWrong";
            break;
        }
        else{

            QString fileName2 = rootAddress+"\\relation.txt";//能打开，就在打开relation表
            QFile file2(fileName2);
            file2.open(QIODevice::ReadWrite| QIODevice::Text);
            QString a=file2.readAll();
            QByteArray array=file2.readAll();
            QStringList strlist = a.split("\n");//以转行符分割信息
            file2.close();
            for(int i=0;i<strlist.size();i++){
                QString b= strlist.at(i);
                  QStringList strlist2 = b.split("-");//再以‘-’分割信息
                  QString eeer= strlist2.at(0);
                  QStringList strlist7 = eeer.split(".");//再以‘.’分割信息
            if(strlist7.at(0).compare(strlist3.at(k),Qt::CaseSensitive)==0){//查询是否有要删除的信息，有就删除
              h=2;
              file.close();
              krr="DropTableFail";
              return krr;
            }
            }
            for(int i=0;i<strlist.size();i++){
                QString b= strlist.at(i);
                  QStringList strlist2 = b.split("-");//再以‘-’分割信息
                  QString eeer= strlist2.at(0);
                  QStringList strlist7 = eeer.split(".");//再以‘.’分割信息
                  QString c= strlist2.at(strlist2.size()-1);
                  QStringList strlist4 = c.split(".");//再以‘.’分割信息

                  if(strlist4.at(0).compare(strlist3.at(k),Qt::CaseSensitive)==0 && strlist7.at(0).compare(strlist3.at(k),Qt::CaseSensitive)!=0){//查询是否有要删除的信息，有就删除
                    QFile::remove(fileName);
                    strlist.removeAt(i);
                    h=1;
                  }

            }


            QFile file3(fileName2);
            file3.open(QIODevice::ReadWrite| QIODevice::Text);
            QTextStream edit(&file3);
            QByteArray array2=file3.readAll();//将修改后的内容重新写入文件
            file3.resize(0);
            for(int i=0;i<strlist.size();i++){
                edit<<strlist.at(i)<<endl;
            }

            file3.close();
            if(h==2) { //没有就返回删除失败
               break;
            }
        }
       file.close();
                }
    if(h==1){
       krr="DropTableOK";
    }

    if(h==2) { //没有就返回删除失败
       krr="DropTableFail";
    }

    return krr;
    }
