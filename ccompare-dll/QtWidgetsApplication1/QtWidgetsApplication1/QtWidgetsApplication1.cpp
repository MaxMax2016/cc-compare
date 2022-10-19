#include "QtWidgetsApplication1.h"
#include <ccmp.h>


QtWidgetsApplication1::QtWidgetsApplication1(QWidget *parent)
    : QMainWindow(parent)
{
    ui.setupUi(this);

	CCmp* pcmp = new CCmp(this);
	
#if 0
	//�ļ�ͬ���ĵ��÷�ʽ���ᵯ���ԱȽ��棬�ɽ���ͬ��
	pcmp->compareSyncFile(QString(".//testfile/1.txt"), QString(".//testfile/2.txt"));
#else
	//�ļ��Աȵķ�ʽ���Լ���ȡ��ͬ��󣬽����Զ�����Ⱦ��ʾ���Ƚ���ɺ󣬻ᷢ��cmpResult�ź�
	connect(pcmp, &CCmp::cmpFinished, this, &QtWidgetsApplication1::on_cmpSuccess);
	pcmp->compareFile(QString(".//testfile/1.txt"), QString(".//testfile/2.txt"));
#endif
}

#if 0
//����������Զ�����ơ�1���򵥵����ӣ�����ıʣ�ʹ�ú�ɫ�ʱ����ͬ����
void QtWidgetsApplication1::on_cmpSuccess(int resultType, QStringList* leftContents, QStringList* rightContents, QVector<UnequalCharsPosInfo>* leftUnequalInfo, QVector<UnequalCharsPosInfo>* rightUnequalInfo, \
	const QList<BlockUserData*>* leftBlockData, const QList<BlockUserData*>* rightBlockData)
{
	if (resultType == 0)
	{
		//�Ա��������أ��Զ��崦����
		//�Ȱ��������ߵ�����,��ʾ����������textbrower�С�

		//��ߵ�ÿһ�ж���leftContents�У��ұ���rightContents��
		QString leftText = leftContents->join("");
		QString rightText = rightContents->join("");

		QByteArray leftUtf8Chars = leftText.toUtf8();
		
		//�������ΰ�textд��leftTextEdit�����ǲ�ͬ�Ĳ��֣�ʹ�ú�ɫ��ɫ��д��
		//ע������Ҫ����utf8�ĸ�ʽ������д�룬leftUnequalInfo�����еĳ��ȶ��ǰ���utf8������ͳһ�滮�ġ�

		const QColor equalColor = QColor("black");
		const QColor unequalColor = QColor("red");

		int curPos = 0;
		for (int i = 0, s = leftUnequalInfo->size(); i < s; ++i)
		{
			const UnequalCharsPosInfo& unequalinfo = leftUnequalInfo->at(i);

			//��д����ͬ�Ĳ���
			if (curPos < unequalinfo.start)
			{
				ui.leftTextEdit->setTextColor(equalColor);
				QString text = QString(leftUtf8Chars.mid(curPos, unequalinfo.start - curPos));
				ui.leftTextEdit->insertPlainText(text);
			}

			if (unequalinfo.length > 0)
			{
				//�л�Ϊ��ʣ���д�벻ͬ����
				ui.leftTextEdit->setTextColor(unequalColor);
				QString text = QString(leftUtf8Chars.mid(unequalinfo.start, unequalinfo.length));
				ui.leftTextEdit->insertPlainText(text);
			}

			curPos = unequalinfo.start + unequalinfo.length;
		}

		//��д���ұߵĲ�ͬ
		curPos = 0;
		QByteArray rightUtf8Chars = rightText.toUtf8();
		for (int i = 0, s = rightUnequalInfo->size(); i < s; ++i)
		{
			const UnequalCharsPosInfo& unequalinfo = rightUnequalInfo->at(i);

			//��д����ͬ�Ĳ���
			if (curPos < unequalinfo.start)
			{
				ui.rightTextEdit->setTextColor(equalColor);
				QString text = QString(rightUtf8Chars.mid(curPos, unequalinfo.start - curPos));
				ui.rightTextEdit->insertPlainText(text);
			}

			if (unequalinfo.length > 0)
			{
				//�л�Ϊ��ʣ���д�벻ͬ����
				ui.rightTextEdit->setTextColor(unequalColor);
				QString text = QString(rightUtf8Chars.mid(unequalinfo.start, unequalinfo.length));
				ui.rightTextEdit->insertPlainText(text);
			}

			curPos = unequalinfo.start + unequalinfo.length;
		}

		//������β�������Ҫ���ա���ܻ��Զ�����

	}
	else if (resultType == 1)
	{
		//����һ���ļ��ǿ����ݵ��ļ�������Ҫ���жԱȡ�
		//ֱ�Ӱ�ԭʼ�ļ���ȡ����ʾ�������ɡ�����ʵ��
	}
}
#else


enum BLOCKSTATUS {
	UNKNOWN_BLOCK = 0, //δ֪
	EQUAL_BLOCK = 1,//���
	UNEQUAL_BLOCK, //����
	PAD_BLOCK, //����
	LAST_PAD_EMPTY_BLOCK, // ���һ�����ڶ���Ŀ��У�ֻ�����һ�п��ܳ���
	TEMP_INSERT_BLOCK //��ʱ�����
};


//����������Զ�����ơ�һ�����ӵ����ӣ����е�ǰ���ע�е����ʣ��кţ�������[pad]��������[x]������ȵȲ���ע��
void QtWidgetsApplication1::on_cmpSuccess(int resultType, QStringList* leftContents, QStringList* rightContents, QVector<UnequalCharsPosInfo>* leftUnequalInfo, QVector<UnequalCharsPosInfo>* rightUnequalInfo, \
	const QList<BlockUserData*>* leftBlockData, const QList<BlockUserData*>* rightBlockData)
{
	if (resultType == 0)
	{
		//�Ա��������أ��Զ��崦����
		//�Ȱ��������ߵ�����,��ʾ����������textbrower�С�

		//�������ΰ�textд��leftTextEdit�����ǲ�ͬ�Ĳ��֣�ʹ�ú�ɫ��ɫ��д��
		//ע������Ҫ����utf8�ĸ�ʽ������д�룬leftUnequalInfo�����еĳ��ȶ��ǰ���utf8������ͳһ�滮�ġ�

		const QColor equalColor = QColor("black");
		const QColor unequalColor = QColor("red");

		int curPos = 0;

		//��Ŀǰ�����еĲ�ͬ�������
		auto getUnEqualBlock = [](int pos, int lineLength, QVector<UnequalCharsPosInfo>* unequalInfo)->QList< UnequalCharsPosInfo > {

			int oldPos = pos;

			QList< UnequalCharsPosInfo >ret;

			for (int i = 0; i < unequalInfo->size() && lineLength >0; ++i)
			{
				const UnequalCharsPosInfo & unequal = unequalInfo->at(i);

				if (pos + lineLength <= unequal.start)
				{
					break;
				}
				if (pos >= unequal.start + unequal.length)
				{
					continue;
				}

				//����й����Σ�ֻ������UnequalCharsPosInfo�����߶��м�����֡��������ǽ����ཻ��
				int a1 = pos;
				int a2 = pos + lineLength;
				int b1 = unequal.start;
				int b2 = unequal.start + unequal.length;

				if (b2 >= a1 && a2 >= b1)
				{
					UnequalCharsPosInfo r;
					r.start = qMax(a1, b1);
					r.length = qMin(a2, b2) - r.start;
					if (r.length > 0)
					{
					r.start = r.start - oldPos;
					ret.append(r);
				}
			}
			}
			return ret;
		};

		//һ��һ�е���д������ı�
		for (int i = 0; i < leftContents->size(); ++i)
		{
			//�Ȳ�ѯ������ʲô��
			int type = leftBlockData->at(i)->m_blockType;

			switch (type)
			{
			case EQUAL_BLOCK://����У�ֱ�������ı�
			{
				ui.leftTextEdit->setTextColor(equalColor);
				ui.leftTextEdit->insertPlainText(QString("%1 ").arg(i) + leftContents->at(i));

				curPos += leftContents->at(i).toUtf8().length();
			}
				break;
			case UNEQUAL_BLOCK://�����У�ǰ�����[x]
			{
				ui.leftTextEdit->setTextColor(unequalColor);
				ui.leftTextEdit->insertPlainText(QString("%1 [x]").arg(i));

				

				QByteArray byte = leftContents->at(i).toUtf8();
				int lineLens = byte.length();

				QList< UnequalCharsPosInfo > unequalInfo = getUnEqualBlock(curPos, byte.length(), leftUnequalInfo);

				if (unequalInfo.isEmpty())
				{
					ui.leftTextEdit->setTextColor(equalColor);
					ui.leftTextEdit->insertPlainText(leftContents->at(i));
				}
				else
				{
					int linePos = 0;
					
					for (int i = 0; i < unequalInfo.size(); ++i)
					{
						const UnequalCharsPosInfo& unequalinfo = unequalInfo.at(i);

						//��д����ͬ�Ĳ���
						if (linePos < unequalinfo.start)
						{
							ui.leftTextEdit->setTextColor(equalColor);
							QString text = QString(byte.mid(linePos, unequalinfo.start - linePos));
							ui.leftTextEdit->insertPlainText(text);
						}

						if (unequalinfo.length > 0)
						{
							//�л�Ϊ��ʣ���д�벻ͬ����
							ui.leftTextEdit->setTextColor(unequalColor);
							QString text = QString(byte.mid(unequalinfo.start, unequalinfo.length));
							ui.leftTextEdit->insertPlainText(text);
						}

						linePos = unequalinfo.start + unequalinfo.length;
					}
					if (linePos < lineLens)
					{
						ui.leftTextEdit->setTextColor(equalColor);
						QString text = QString(byte.mid(linePos,-1));
						ui.leftTextEdit->insertPlainText(text);
					}
				}

				curPos += lineLens;
			}
				break;
			case PAD_BLOCK://�����У�ǰ�����[pad]
				ui.leftTextEdit->setTextColor(unequalColor);
				ui.leftTextEdit->insertPlainText(QString("%1 [pad]").arg(i) + leftContents->at(i));
				curPos += leftContents->at(i).toUtf8().length();
				break;
			case LAST_PAD_EMPTY_BLOCK://���ܴ��ڵ����һ�������У�ǰ�����[lastpad]
				ui.leftTextEdit->insertPlainText(QString("%1 [lastpad]").arg(i) + leftContents->at(i));
				curPos += leftContents->at(i).toUtf8().length();
				break;
			default:
				break;
			}
		}
		
		curPos = 0;
		//һ��һ�е���д���ұ��ı�
		for (int i = 0; i < rightContents->size(); ++i)
		{
			//�Ȳ�ѯ������ʲô��
			int type = rightBlockData->at(i)->m_blockType;

			switch (type)
			{
			case EQUAL_BLOCK://����У�ֱ�������ı�
			{
				ui.rightTextEdit->setTextColor(equalColor);
				ui.rightTextEdit->insertPlainText(QString("%1 ").arg(i) + rightContents->at(i));

				curPos += rightContents->at(i).toUtf8().length();
			}
			break;
			case UNEQUAL_BLOCK://�����У�ǰ�����[x]
			{
				ui.rightTextEdit->setTextColor(unequalColor);
				ui.rightTextEdit->insertPlainText(QString("%1 [x]").arg(i));

				QByteArray byte = rightContents->at(i).toUtf8();
				QList< UnequalCharsPosInfo > unequalInfo = getUnEqualBlock(curPos, byte.length(), rightUnequalInfo);
				int lineLens = byte.length();;

				if (unequalInfo.isEmpty())
				{
					ui.rightTextEdit->setTextColor(equalColor);
					ui.rightTextEdit->insertPlainText(rightContents->at(i));
				}
				else
				{
					int linePos = 0;
					for (int i = 0; i < unequalInfo.size(); ++i)
					{
						const UnequalCharsPosInfo& unequalinfo = unequalInfo.at(i);

						//��д����ͬ�Ĳ���
						if (linePos < unequalinfo.start)
						{
							ui.rightTextEdit->setTextColor(equalColor);
							QString text = QString(byte.mid(linePos, unequalinfo.start - linePos));
							ui.rightTextEdit->insertPlainText(text);
						}

						if (unequalinfo.length > 0)
						{
							//�л�Ϊ��ʣ���д�벻ͬ����
							ui.rightTextEdit->setTextColor(unequalColor);
							QString text = QString(byte.mid(unequalinfo.start, unequalinfo.length));
							ui.rightTextEdit->insertPlainText(text);
						}

						linePos = unequalinfo.start + unequalinfo.length;
					}
					if (linePos < lineLens)
					{
						ui.rightTextEdit->setTextColor(equalColor);
						QString text = QString(byte.mid(linePos, -1));
						ui.rightTextEdit->insertPlainText(text);
					}
				}
				
				curPos += lineLens;
			}
			break;
			case PAD_BLOCK://�����У�ǰ�����[pad]
				ui.rightTextEdit->setTextColor(unequalColor);
				ui.rightTextEdit->insertPlainText(QString("%1 [pad]").arg(i) + rightContents->at(i));
				curPos += rightContents->at(i).toUtf8().length();
				break;
			case LAST_PAD_EMPTY_BLOCK://���ܴ��ڵ����һ�������У�ǰ�����[lastpad]
				ui.rightTextEdit->insertPlainText(QString("%1 [lastpad]").arg(i) + rightContents->at(i));
				curPos += rightContents->at(i).toUtf8().length();
				break;
			default:
				break;
			}
		}

	}
	else if (resultType == 1)
	{
		//����һ���ļ��ǿ����ݵ��ļ�������Ҫ���жԱȡ�
		//ֱ�Ӱ�ԭʼ�ļ���ȡ����ʾ�������ɡ�����ʵ��
	}
}
#endif
