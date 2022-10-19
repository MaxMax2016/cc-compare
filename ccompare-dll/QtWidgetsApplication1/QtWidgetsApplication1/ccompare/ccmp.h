#pragma once
#include <QString>
#include <qobject.h>
#include <QVector>
#include "blockuserdata.h"


#if defined(CC_EXPORTDLL)
#define CC_EXPORT __declspec(dllexport)
#else
#define CC_EXPORT __declspec(dllimport)
#endif

typedef struct UnequalCharsPosInfo_ {
	int start;
	int length;
}UnequalCharsPosInfo;

class CC_EXPORT CCmp : public QObject
{
	Q_OBJECT
public:
	CCmp(QObject* parent=nullptr);
	virtual ~CCmp();

	void setCmpMode(int mode);// 0 Ĭ��ֵ,������ǰ�кſհ��ַ� 1:ֻ������β�հ��ַ� 2:������ǰ��β�����пհ�
	void setCmpParameter(bool isBlankLineDoMatch, int lineMatchEqualRata);//�ԱȲ�����isBlankLineDoMatch �����Ƿ����Ƚ�Ĭ��true��lineMatchEqualRata ���϶�Ϊ��ȵ�������Ĭ��50

	//�Ա�ͬ���ļ�
	void compareSyncFile(QString leftPath, QString rightPath);

	//�Ա��ļ����ԱȽ������cmpResult���ź����
	QObject* compareFile(QString leftPath, QString rightPath);

signals:
	void cmpFinished(int resultType, QStringList* leftContents, QStringList* rightContents, QVector<UnequalCharsPosInfo>* leftUnequalInfo, QVector<UnequalCharsPosInfo>* rightUnequalInfo, \
		const QList<BlockUserData*>* leftBlockData, const QList<BlockUserData*>* rightBlockData);

private:
	int m_mode;
	bool m_isBlankLineDoMatch;
	int m_lineMatchEqualRata;
};

