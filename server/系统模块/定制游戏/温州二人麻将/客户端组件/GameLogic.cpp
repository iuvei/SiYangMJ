#include "StdAfx.h"
#include "GameLogic.h"
#include "math.h"


//////////////////////////////////////////////////////////////////////////

//静态变量
bool    CChiHuRight::m_bInit = false;
DWORD   CChiHuRight::m_dwRightMask[MAX_RIGHT_COUNT];

//构造函数
CChiHuRight::CChiHuRight()
{
	ZeroMemory(m_dwRight,sizeof(m_dwRight));

	if(!m_bInit)
	{
		m_bInit = true;
		for(BYTE i = 0; i < CountArray(m_dwRightMask); i++)
		{
			if(0 == i)
			{
				m_dwRightMask[i] = 0;
			}
			else
			{
				m_dwRightMask[i] = (DWORD(pow((float)2,(float)(i-1))))<<28;
			}
		}
	}
}

//赋值符重载
CChiHuRight & CChiHuRight::operator = (DWORD dwRight)
{
	DWORD dwOtherRight = 0;
	//验证权位
	if(!IsValidRight(dwRight))
	{
		//验证取反权位
		ASSERT(IsValidRight(~dwRight));
		if(!IsValidRight(~dwRight))
		{
			return *this;
		}
		dwRight = ~dwRight;
		dwOtherRight = MASK_CHI_HU_RIGHT;
	}

	for(BYTE i = 0; i < CountArray(m_dwRightMask); i++)
	{
		if((dwRight&m_dwRightMask[i]) || (i==0&&dwRight<0x10000000))
		{
			m_dwRight[i] = dwRight&MASK_CHI_HU_RIGHT;
		}
		else
		{
			m_dwRight[i] = dwOtherRight;
		}
	}

	return *this;
}

//与等于
CChiHuRight & CChiHuRight::operator &= (DWORD dwRight)
{
	bool bNavigate = false;
	//验证权位
	if(!IsValidRight(dwRight))
	{
		//验证取反权位
		ASSERT(IsValidRight(~dwRight));
		if(!IsValidRight(~dwRight))
		{
			return *this;
		}
		//调整权位
		DWORD dwHeadRight = (~dwRight)&0xF0000000;
		DWORD dwTailRight = dwRight&MASK_CHI_HU_RIGHT;
		dwRight = dwHeadRight|dwTailRight;
		bNavigate = true;
	}

	for(BYTE i = 0; i < CountArray(m_dwRightMask); i++)
	{
		if((dwRight&m_dwRightMask[i]) || (i==0&&dwRight<0x10000000))
		{
			m_dwRight[i] &= (dwRight&MASK_CHI_HU_RIGHT);
		}
		else if(!bNavigate)
		{
			m_dwRight[i] = 0;
		}
	}

	return *this;
}

//或等于
CChiHuRight & CChiHuRight::operator |= (DWORD dwRight)
{
	//验证权位
	if(!IsValidRight(dwRight))
	{
		return *this;
	}

	for(BYTE i = 0; i < CountArray(m_dwRightMask); i++)
	{
		if((dwRight&m_dwRightMask[i]) || (i==0&&dwRight<0x10000000))
		{
			m_dwRight[i] |= (dwRight&MASK_CHI_HU_RIGHT);
			break;
		}
	}

	return *this;
}

//与
CChiHuRight CChiHuRight::operator & (DWORD dwRight)
{
	CChiHuRight chr = *this;
	return (chr &= dwRight);
}

//与
CChiHuRight CChiHuRight::operator & (DWORD dwRight) const
{
	CChiHuRight chr = *this;
	return (chr &= dwRight);
}

//或
CChiHuRight CChiHuRight::operator | (DWORD dwRight)
{
	CChiHuRight chr = *this;
	return chr |= dwRight;
}

//或
CChiHuRight CChiHuRight::operator | (DWORD dwRight) const
{
	CChiHuRight chr = *this;
	return chr |= dwRight;
}

//相等
bool CChiHuRight::operator == (DWORD dwRight) const
{
	CChiHuRight chr;
	chr = dwRight;
	return (*this)==chr;
}

//相等
bool CChiHuRight::operator == (const CChiHuRight chr) const
{
	for(WORD i = 0; i < CountArray(m_dwRight); i++)
	{
		if(m_dwRight[i] != chr.m_dwRight[i])
		{
			return false;
		}
	}
	return true;
}

//不相等
bool CChiHuRight::operator != (DWORD dwRight) const
{
	CChiHuRight chr;
	chr = dwRight;
	return (*this)!=chr;
}

//不相等
bool CChiHuRight::operator != (const CChiHuRight chr) const
{
	return !((*this)==chr);
}

//是否权位为空
bool CChiHuRight::IsEmpty()
{
	for(BYTE i = 0; i < CountArray(m_dwRight); i++)
		if(m_dwRight[i])
		{
			return false;
		}
		return true;
}

//设置权位为空
void CChiHuRight::SetEmpty()
{
	ZeroMemory(m_dwRight,sizeof(m_dwRight));
	return;
}

//获取权位数值
BYTE CChiHuRight::GetRightData(DWORD dwRight[], BYTE cbMaxCount)
{
	ASSERT(cbMaxCount >= CountArray(m_dwRight));
	if(cbMaxCount < CountArray(m_dwRight))
	{
		return 0;
	}

	CopyMemory(dwRight,m_dwRight,sizeof(DWORD)*CountArray(m_dwRight));
	return CountArray(m_dwRight);
}

//设置权位数值
bool CChiHuRight::SetRightData(const DWORD dwRight[], BYTE cbRightCount)
{
	ASSERT(cbRightCount <= CountArray(m_dwRight));
	if(cbRightCount > CountArray(m_dwRight))
	{
		return false;
	}

	ZeroMemory(m_dwRight,sizeof(m_dwRight));
	CopyMemory(m_dwRight,dwRight,sizeof(DWORD)*cbRightCount);

	return true;
}

//检查仅位是否正确
bool CChiHuRight::IsValidRight(DWORD dwRight)
{
	DWORD dwRightHead = dwRight & 0xF0000000;
	for(BYTE i = 0; i < CountArray(m_dwRightMask); i++)
		if(m_dwRightMask[i] == dwRightHead)
		{
			return true;
		}
		return false;
}

//////////////////////////////////////////////////////////////////////////

//静态变量

//扑克数据
const BYTE CGameLogic::m_cbCardDataArray[MAX_REPERTORY]=
{
	0x01,0x02,0x03,0x04,0x05,0x06,0x07,0x08,0x09,           //万子
	0x01,0x02,0x03,0x04,0x05,0x06,0x07,0x08,0x09,           //万子
	0x01,0x02,0x03,0x04,0x05,0x06,0x07,0x08,0x09,           //万子
	0x01,0x02,0x03,0x04,0x05,0x06,0x07,0x08,0x09,           //万子
	0x11,0x12,0x13,0x14,0x15,0x16,0x17,0x18,0x19,           //索子
	0x11,0x12,0x13,0x14,0x15,0x16,0x17,0x18,0x19,           //索子
	0x11,0x12,0x13,0x14,0x15,0x16,0x17,0x18,0x19,           //索子
	0x11,0x12,0x13,0x14,0x15,0x16,0x17,0x18,0x19,           //索子
	0x21,0x22,0x23,0x24,0x25,0x26,0x27,0x28,0x29,           //同子
	0x21,0x22,0x23,0x24,0x25,0x26,0x27,0x28,0x29,           //同子
	0x21,0x22,0x23,0x24,0x25,0x26,0x27,0x28,0x29,           //同子
	0x21,0x22,0x23,0x24,0x25,0x26,0x27,0x28,0x29,           //同子
	0x31,0x32,0x33,0x34,0x35,0x36,0x37,                 //番子
	0x31,0x32,0x33,0x34,0x35,0x36,0x37,                 //番子
	0x31,0x32,0x33,0x34,0x35,0x36,0x37,                 //番子
	0x31,0x32,0x33,0x34,0x35,0x36,0x37,                 //番子
};

//////////////////////////////////////////////////////////////////////////

//构造函数
CGameLogic::CGameLogic()
{
	m_cbMagicIndex = MAX_INDEX;
}

//析构函数
CGameLogic::~CGameLogic()
{
}

//混乱扑克
void CGameLogic::RandCardData(BYTE cbCardData[], BYTE cbMaxCount)
{
	//混乱准备
	BYTE cbCardDataTemp[CountArray(m_cbCardDataArray)];
	CopyMemory(cbCardDataTemp,m_cbCardDataArray,sizeof(m_cbCardDataArray));

	//混乱扑克
	BYTE cbRandCount=0,cbPosition=0;
	do
	{
		cbPosition=rand()%(cbMaxCount-cbRandCount);
		cbCardData[cbRandCount++]=cbCardDataTemp[cbPosition];
		cbCardDataTemp[cbPosition]=cbCardDataTemp[cbMaxCount-cbRandCount];
	}
	while(cbRandCount<cbMaxCount);

	return;
}

//混乱扑克
void CGameLogic::RandCardData(const BYTE cbCardData[], BYTE cbCardCount, BYTE cbRandData[])
{
	if(cbCardCount == 0)
	{
		return;
	}

	//混乱准备
	BYTE cbCardDataTemp[MAX_COUNT];
	CopyMemory(cbCardDataTemp,cbCardData,sizeof(BYTE)*cbCardCount);

	//混乱扑克
	BYTE cbRandCount=0,cbPosition=0;
	do
	{
		cbPosition=rand()%(cbCardCount-cbRandCount);
		cbRandData[cbRandCount++]=cbCardDataTemp[cbPosition];
		cbCardDataTemp[cbPosition]=cbCardDataTemp[cbCardCount-cbRandCount];
	}
	while(cbRandCount<cbCardCount);

	return;
}

//删除扑克
bool CGameLogic::RemoveCard(BYTE cbCardIndex[MAX_INDEX], BYTE cbRemoveCard)
{
	//效验扑克
	ASSERT(IsValidCard(cbRemoveCard));
	ASSERT(cbCardIndex[SwitchToCardIndex(cbRemoveCard)]>0);

	//删除扑克
	BYTE cbRemoveIndex=SwitchToCardIndex(cbRemoveCard);
	if(cbCardIndex[cbRemoveIndex]>0)
	{
		cbCardIndex[cbRemoveIndex]--;
		return true;
	}

	//失败效验
	ASSERT(FALSE);

	return false;
}

//删除扑克
bool CGameLogic::RemoveCard(BYTE cbCardIndex[MAX_INDEX], const BYTE cbRemoveCard[], BYTE cbRemoveCount)
{
	//删除扑克
	for(BYTE i=0; i<cbRemoveCount; i++)
	{
		//效验扑克
		ASSERT(IsValidCard(cbRemoveCard[i]));
		ASSERT(cbCardIndex[SwitchToCardIndex(cbRemoveCard[i])]>0);

		//删除扑克
		BYTE cbRemoveIndex=SwitchToCardIndex(cbRemoveCard[i]);
		if(cbCardIndex[cbRemoveIndex]==0)
		{
			//错误断言
			ASSERT(FALSE);

			//还原删除
			for(BYTE j=0; j<i; j++)
			{
				ASSERT(IsValidCard(cbRemoveCard[j]));
				cbCardIndex[SwitchToCardIndex(cbRemoveCard[j])]++;
			}

			return false;
		}
		else
		{
			//删除扑克
			--cbCardIndex[cbRemoveIndex];
		}
	}

	return true;
}

//删除扑克
bool CGameLogic::RemoveCard(BYTE cbCardData[], BYTE cbCardCount, const BYTE cbRemoveCard[], BYTE cbRemoveCount)
{
	//检验数据
	ASSERT(cbCardCount<=MAX_COUNT);
	ASSERT(cbRemoveCount<=cbCardCount);

	//定义变量
	BYTE cbDeleteCount=0,cbTempCardData[MAX_COUNT];
	if(cbCardCount>CountArray(cbTempCardData))
	{
		return false;
	}
	CopyMemory(cbTempCardData,cbCardData,cbCardCount*sizeof(cbCardData[0]));

	//置零扑克
	for(BYTE i=0; i<cbRemoveCount; i++)
	{
		for(BYTE j=0; j<cbCardCount; j++)
		{
			if(cbRemoveCard[i]==cbTempCardData[j])
			{
				cbDeleteCount++;
				cbTempCardData[j]=0;
				break;
			}
		}
	}

	//成功判断
	if(cbDeleteCount!=cbRemoveCount)
	{
		ASSERT(FALSE);
		return false;
	}

	//清理扑克
	BYTE cbCardPos=0;
	for(BYTE i=0; i<cbCardCount; i++)
	{
		if(cbTempCardData[i]!=0)
		{
			cbCardData[cbCardPos++]=cbTempCardData[i];
		}
	}

	return true;
}

//有效判断
bool CGameLogic::IsValidCard(BYTE cbCardData)
{
	BYTE cbValue=(cbCardData&MASK_VALUE);
	BYTE cbColor=(cbCardData&MASK_COLOR)>>4;
	return (((cbValue>=1)&&(cbValue<=9)&&(cbColor<=2))||((cbValue>=1)&&(cbValue<=0x0f)&&(cbColor==3)));
}

//扑克数目
BYTE CGameLogic::GetCardCount(const BYTE cbCardIndex[MAX_INDEX])
{
	//数目统计
	BYTE cbCardCount=0;
	for(BYTE i=0; i<MAX_INDEX; i++)
	{
		cbCardCount+=cbCardIndex[i];
	}

	return cbCardCount;
}

//获取组合
BYTE CGameLogic::GetWeaveCard(BYTE cbWeaveKind, BYTE cbCenterCard, BYTE cbCardBuffer[4])
{
	//组合扑克
	switch(cbWeaveKind)
	{
	case WIK_LEFT:    //上牌操作
		{
			//设置变量
			cbCardBuffer[0]=cbCenterCard;
			cbCardBuffer[1]=cbCenterCard+1;
			cbCardBuffer[2]=cbCenterCard+2;

			return 3;
		}
	case WIK_RIGHT:   //上牌操作
		{
			//设置变量
			cbCardBuffer[0]=cbCenterCard-2;
			cbCardBuffer[1]=cbCenterCard-1;
			cbCardBuffer[2]=cbCenterCard;

			return 3;
		}
	case WIK_CENTER:  //上牌操作
		{
			//设置变量
			cbCardBuffer[0]=cbCenterCard-1;
			cbCardBuffer[1]=cbCenterCard;
			cbCardBuffer[2]=cbCenterCard+1;

			return 3;
		}
	case WIK_PENG:    //碰牌操作
		{
			//设置变量
			cbCardBuffer[0]=cbCenterCard;
			cbCardBuffer[1]=cbCenterCard;
			cbCardBuffer[2]=cbCenterCard;

			return 3;
		}
	case WIK_GANG:    //杠牌操作
		{
			//设置变量
			cbCardBuffer[0]=cbCenterCard;
			cbCardBuffer[1]=cbCenterCard;
			cbCardBuffer[2]=cbCenterCard;
			cbCardBuffer[3]=cbCenterCard;

			return 4;
		}
	default:
		{
			ASSERT(FALSE);
		}
	}

	return 0;
}

//花牌数目
BYTE CGameLogic::GetMagicCardCount(const BYTE cbCardIndex[MAX_INDEX])
{
	if(m_cbMagicIndex >= MAX_INDEX)
	{
		return 0;
	}
	if(m_cbMagicIndex >= 0 && m_cbMagicIndex < MAX_INDEX-MAX_HUA_CARD)
	{
		return cbCardIndex[m_cbMagicIndex];
	}

	BYTE cbMagicCardCount = 0;
	if(m_cbMagicIndex >= MAX_INDEX-MAX_HUA_CARD && m_cbMagicIndex < MAX_INDEX-MAX_HUA_CARD+4)
	{
		cbMagicCardCount = cbCardIndex[34]+cbCardIndex[35]+cbCardIndex[36]+cbCardIndex[37];
	}
	else if(m_cbMagicIndex >= MAX_INDEX-MAX_HUA_CARD+4 && m_cbMagicIndex < MAX_INDEX)
	{
		cbMagicCardCount = cbCardIndex[38]+cbCardIndex[39]+cbCardIndex[40]+cbCardIndex[41];
	}
	else
	{
		ASSERT(FALSE);
	}

	return cbMagicCardCount;
}

//动作等级
BYTE CGameLogic::GetUserActionRank(BYTE cbUserAction)
{
	//胡牌等级
	if(cbUserAction&WIK_CHI_HU)
	{
		return 4;
	}

	//杠牌等级
	if(cbUserAction&WIK_GANG)
	{
		return 3;
	}

	//碰牌等级
	if(cbUserAction&WIK_PENG)
	{
		return 2;
	}

	//上牌等级
	if(cbUserAction&(WIK_RIGHT|WIK_CENTER|WIK_LEFT))
	{
		return 1;
	}

	return 0;
}

//胡牌等级
BYTE CGameLogic::GetChiHuActionRank(const CChiHuRight & ChiHuRight, bool bCheckFan/* = false*/)
{
	//4番 特殊牌型 天胡 地胡 硬八对 单钓 三财神
	if(!(ChiHuRight&CHR_BA_DUI_YING).IsEmpty() ||
		!(ChiHuRight&CHR_TIAN_HU).IsEmpty() ||
		!(ChiHuRight&CHR_DI_HU).IsEmpty() ||
		!(ChiHuRight&CHR_THREE_KING).IsEmpty())
	{
		return 4;
	}

	//2番 硬牌 八对
	if(!(ChiHuRight&CHR_BA_DUI).IsEmpty() ||
		!(ChiHuRight&CHR_YING_HU).IsEmpty())
	{
		return 2;
	}

	//1番 其余都是软胡
	return 1;
}

//吃牌判断
BYTE CGameLogic::EstimateEatCard(const BYTE cbCardIndex[MAX_INDEX], BYTE cbCurrentCard)
{
	//参数效验
	ASSERT(IsValidCard(cbCurrentCard));

	//过滤判断
	if((cbCurrentCard>=0x31 && cbCurrentCard<=0x36) || (cbCurrentCard==0x37 && m_cbMagicIndex>=27) || IsMagicCard(cbCurrentCard, false))
	{
		return WIK_NULL;
	}

	//变量定义
	BYTE cbExcursion[3]= {0,1,2};
	BYTE cbItemKind[3]= {WIK_LEFT,WIK_CENTER,WIK_RIGHT};
	BYTE cbInsteadCard = cbCurrentCard;
	BYTE cbInsteadIndex[MAX_INDEX];
	ZeroMemory(cbInsteadIndex,sizeof(cbInsteadIndex));
	memcpy(cbInsteadIndex, cbCardIndex, sizeof(cbInsteadIndex));

	//替换白板
	if(m_cbMagicIndex != MAX_INDEX && SwitchToCardData(m_cbMagicIndex) != 0x37)
	{
		//当前牌
		if(cbCurrentCard == 0x37 && !IsMagicCard(cbCurrentCard, false))
		{
			cbInsteadCard = SwitchToCardData(m_cbMagicIndex);
		}

		//手中牌
		cbInsteadIndex[m_cbMagicIndex] = cbInsteadIndex[33];
		cbInsteadIndex[33] = 0;
	}

	//吃牌判断
	BYTE cbEatKind=0,cbFirstIndex=0;
	BYTE cbCurrentIndex=SwitchToCardIndex(cbInsteadCard);
	for(BYTE i=0; i<CountArray(cbItemKind); i++)
	{
		BYTE cbValueIndex=cbCurrentIndex%9;
		if((cbValueIndex>=cbExcursion[i])&&((cbValueIndex-cbExcursion[i])<=6))
		{
			//吃牌判断
			cbFirstIndex=cbCurrentIndex-cbExcursion[i];

			//吃牌不能包含有王霸
			/*if( m_cbMagicIndex != MAX_INDEX &&
			m_cbMagicIndex >= cbFirstIndex && m_cbMagicIndex <= cbFirstIndex+2 ) continue;*/

			if((cbCurrentIndex!=cbFirstIndex)&&(cbInsteadIndex[cbFirstIndex]==0))
			{
				continue;
			}
			if((cbCurrentIndex!=(cbFirstIndex+1))&&(cbInsteadIndex[cbFirstIndex+1]==0))
			{
				continue;
			}
			if((cbCurrentIndex!=(cbFirstIndex+2))&&(cbInsteadIndex[cbFirstIndex+2]==0))
			{
				continue;
			}

			//设置类型
			cbEatKind|=cbItemKind[i];
		}
	}

	return cbEatKind;
}

//碰牌判断
BYTE CGameLogic::EstimatePengCard(const BYTE cbCardIndex[MAX_INDEX], BYTE cbCurrentCard)
{
	//参数效验
	ASSERT(IsValidCard(cbCurrentCard));

	//过滤判断
	if(IsMagicCard(cbCurrentCard, false) || IsHuaCard(cbCurrentCard))
	{
		return WIK_NULL;
	}

	//碰牌判断
	return (cbCardIndex[SwitchToCardIndex(cbCurrentCard)]>=2)?WIK_PENG:WIK_NULL;
}

//杠牌判断
BYTE CGameLogic::EstimateGangCard(const BYTE cbCardIndex[MAX_INDEX], BYTE cbCurrentCard)
{
	//参数效验
	ASSERT(IsValidCard(cbCurrentCard));

	//过滤判断
	if(IsMagicCard(cbCurrentCard, false) || IsHuaCard(cbCurrentCard))
	{
		return WIK_NULL;
	}

	//杠牌判断
	return (cbCardIndex[SwitchToCardIndex(cbCurrentCard)]==3)?WIK_GANG:WIK_NULL;
}

//杠牌分析
BYTE CGameLogic::AnalyseGangCard(const BYTE cbCardIndex[MAX_INDEX], const tagWeaveItem WeaveItem[], BYTE cbWeaveCount, tagGangCardResult & GangCardResult)
{
	//设置变量
	BYTE cbActionMask=WIK_NULL;
	ZeroMemory(&GangCardResult,sizeof(GangCardResult));

	//手上杠牌
	for(BYTE i=0; i<MAX_INDEX; i++)
	{
		if(i == m_cbMagicIndex)
		{
			continue;
		}
		if(cbCardIndex[i]==4)
		{
			cbActionMask|=WIK_GANG;
			GangCardResult.cbCardData[GangCardResult.cbCardCount++]=SwitchToCardData(i);
		}
	}

	//组合杠牌
	for(BYTE i=0; i<cbWeaveCount; i++)
	{
		if(WeaveItem[i].cbWeaveKind==WIK_PENG)
		{
			if(cbCardIndex[SwitchToCardIndex(WeaveItem[i].cbCenterCard)]==1)
			{
				cbActionMask|=WIK_GANG;
				GangCardResult.cbCardData[GangCardResult.cbCardCount++]=WeaveItem[i].cbCenterCard;
			}
		}
	}

	return cbActionMask;
}

//吃胡分析
BYTE CGameLogic::AnalyseChiHuCard(const BYTE cbCardIndex[MAX_INDEX], const tagWeaveItem WeaveItem[], BYTE cbWeaveCount, BYTE cbCurrentCard, CChiHuRight &ChiHuRight)
{
	//变量定义
	BYTE cbChiHuKind=WIK_NULL;
	CAnalyseItemArray AnalyseItemArray;

	//设置变量
	AnalyseItemArray.RemoveAll();
	ChiHuRight.SetEmpty();

	//构造扑克
	BYTE cbCardIndexTemp[MAX_INDEX];
	CopyMemory(cbCardIndexTemp,cbCardIndex,sizeof(cbCardIndexTemp));

	//cbCurrentCard一定不为0      !!!!!!!!!
	ASSERT(cbCurrentCard != 0);
	if(cbCurrentCard == 0)
	{
		return WIK_NULL;
	}

	//插入扑克
	if(cbCurrentCard!=0)
	{
		cbCardIndexTemp[SwitchToCardIndex(cbCurrentCard)]++;
	}

	//特殊番型
	bool bYingBaDui = false;
	if(IsBaDui(cbCardIndexTemp,WeaveItem,cbWeaveCount, bYingBaDui))
	{
		if(bYingBaDui)
		{
			ChiHuRight |= CHR_BA_DUI_YING;
		}
		else
		{
			ChiHuRight |= CHR_BA_DUI;
		}
	}
	if(IsSanJinDao(cbCardIndex))
	{
		ChiHuRight |= CHR_THREE_KING;
	}

	//分析扑克
	AnalyseCard(cbCardIndexTemp,WeaveItem,cbWeaveCount,AnalyseItemArray);

	//胡牌分析
	if(AnalyseItemArray.GetCount()>0)
	{
		//牌型分析
		for(INT_PTR i=0; i<AnalyseItemArray.GetCount(); i++)
		{
			//变量定义
			tagAnalyseItem * pAnalyseItem=&AnalyseItemArray[i];

			/*
			//  判断番型
			*/
			//碰碰胡
			//if( IsPengPeng(pAnalyseItem) )
			//  ChiHuRight |= CHR_PENG_PENG_HU;
			//硬胡软胡
			if(IsYingHu(pAnalyseItem, cbWeaveCount))
			{
				ChiHuRight |= CHR_YING_HU;
			}
			else
			{
				ChiHuRight |= CHR_RUAN_HU;
			}
		}

		cbChiHuKind = WIK_CHI_HU;
	}

	if(!ChiHuRight.IsEmpty())
	{
		cbChiHuKind = WIK_CHI_HU;
	}

	if(cbChiHuKind == WIK_CHI_HU)
	{
		/*
		//  判断番型
		*/
	}

	return cbChiHuKind;
}

//听牌分析
BYTE CGameLogic::AnalyseTingCard(const BYTE cbCardIndex[MAX_INDEX], const tagWeaveItem WeaveItem[], BYTE cbWeaveCount)
{
	//复制数据
	BYTE cbCardIndexTemp[MAX_INDEX];
	CopyMemory(cbCardIndexTemp,cbCardIndex,sizeof(cbCardIndexTemp));

	CChiHuRight chr;
	for(BYTE i = 0; i < MAX_INDEX-MAX_HUA_CARD; i++)
	{
		if(cbCardIndexTemp[i] == 0)
		{
			continue;
		}
		cbCardIndexTemp[i]--;

		for(BYTE j = 0; j < MAX_INDEX-MAX_HUA_CARD; j++)
		{
			BYTE cbCurrentCard = SwitchToCardData(j);
			if(WIK_CHI_HU == AnalyseChiHuCard(cbCardIndexTemp,WeaveItem,cbWeaveCount,cbCurrentCard,chr))
			{
				return WIK_LISTEN;
			}
		}

		cbCardIndexTemp[i]++;
	}

	return WIK_NULL;
}

//扑克转换
BYTE CGameLogic::SwitchToCardData(BYTE cbCardIndex)
{
	ASSERT(cbCardIndex<MAX_INDEX);
	if(cbCardIndex < 27)
	{
		return ((cbCardIndex/9)<<4)|(cbCardIndex%9+1);
	}
	else
	{
		return (0x30|(cbCardIndex-27+1));
	}
}

//扑克转换
BYTE CGameLogic::SwitchToCardIndex(BYTE cbCardData)
{
	ASSERT(IsValidCard(cbCardData));
	return ((cbCardData&MASK_COLOR)>>4)*9+(cbCardData&MASK_VALUE)-1;
}

//扑克转换
BYTE CGameLogic::SwitchToCardData(const BYTE cbCardIndex[MAX_INDEX], BYTE cbCardData[MAX_COUNT])
{
	//转换扑克
	BYTE cbPosition=0;
	//钻牌
	if(m_cbMagicIndex != MAX_INDEX)
	{
		//091231JJ
		if(m_cbMagicIndex >= MAX_INDEX-MAX_HUA_CARD && m_cbMagicIndex < MAX_INDEX)
		{
			for(int i = MAX_INDEX-MAX_HUA_CARD; i < MAX_INDEX; i++)
			{
				if(cbCardIndex[i] > 0 && IsMagicCard(i,true))
				{
					cbCardData[cbPosition++] = SwitchToCardData(i);
				}
			}
		}
		else
		{
			for(BYTE i = 0; i < cbCardIndex[m_cbMagicIndex]; i++)
			{
				cbCardData[cbPosition++] = SwitchToCardData(m_cbMagicIndex);
			}
		}
	}
	for(BYTE i=0; i<MAX_INDEX; i++)
	{
		if(IsMagicCard(i,true) && m_cbMagicIndex != INDEX_REPLACE_CARD)
		{
			//如果财神有代替牌，则代替牌代替财神原来的位置
			if(INDEX_REPLACE_CARD != MAX_INDEX)
			{
				for(BYTE j = 0; j < cbCardIndex[INDEX_REPLACE_CARD]; j++)
				{
					cbCardData[cbPosition++] = SwitchToCardData(INDEX_REPLACE_CARD);
				}
			}
			continue;
		}
		if(i == INDEX_REPLACE_CARD)
		{
			continue;
		}
		if(cbCardIndex[i]!=0)
		{
			for(BYTE j=0; j<cbCardIndex[i]; j++)
			{
				ASSERT(cbPosition<MAX_COUNT);
				cbCardData[cbPosition++]=SwitchToCardData(i);
			}
		}
	}

	return cbPosition;
}

//扑克转换
BYTE CGameLogic::SwitchToCardIndex(const BYTE cbCardData[], BYTE cbCardCount, BYTE cbCardIndex[MAX_INDEX])
{
	//设置变量
	ZeroMemory(cbCardIndex,sizeof(BYTE)*MAX_INDEX);

	//转换扑克
	for(BYTE i=0; i<cbCardCount; i++)
	{
		ASSERT(IsValidCard(cbCardData[i]));
		cbCardIndex[SwitchToCardIndex(cbCardData[i])]++;
	}

	return cbCardCount;
}

//分析扑克
bool CGameLogic::AnalyseCard(const BYTE cbCardIndex[MAX_INDEX], const tagWeaveItem WeaveItem[], BYTE cbWeaveCount, CAnalyseItemArray & AnalyseItemArray)
{
	//计算数目
	BYTE cbCardCount=GetCardCount(cbCardIndex);

	//效验数目
	ASSERT((cbCardCount>=2)&&(cbCardCount<=MAX_COUNT)&&((cbCardCount-2)%3==0));
	if((cbCardCount<2)||(cbCardCount>MAX_COUNT)||((cbCardCount-2)%3!=0))
	{
		return false;
	}

	//变量定义
	BYTE cbKindItemCount=0;
	tagKindItem KindItem[27*2+28];
	ZeroMemory(KindItem,sizeof(KindItem));

	//需求判断
	BYTE cbLessKindItem=(cbCardCount-2)/3;
	ASSERT((cbLessKindItem+cbWeaveCount)==MAX_WEAVE);

	//单吊判断
	if(cbLessKindItem==0)
	{
		//效验参数
		ASSERT((cbCardCount==2)&&(cbWeaveCount==MAX_WEAVE));

		//牌眼判断
		for(BYTE i=0; i<MAX_INDEX-MAX_HUA_CARD; i++)
		{
			//091231JJ
			if(cbCardIndex[i]==2 || (m_cbMagicIndex!=MAX_INDEX && !IsMagicCard(i,true) && GetMagicCardCount(cbCardIndex)+cbCardIndex[i]==2))
				//( m_cbMagicIndex != MAX_INDEX && i != m_cbMagicIndex && cbCardIndex[m_cbMagicIndex]+cbCardIndex[i]==2 ) )
			{
				//变量定义
				tagAnalyseItem AnalyseItem;
				ZeroMemory(&AnalyseItem,sizeof(AnalyseItem));

				//设置结果
				for(BYTE j=0; j<cbWeaveCount; j++)
				{
					AnalyseItem.cbWeaveKind[j]=WeaveItem[j].cbWeaveKind;
					AnalyseItem.cbCenterCard[j]=WeaveItem[j].cbCenterCard;
					CopyMemory(AnalyseItem.cbCardData[j],WeaveItem[j].cbCardData,sizeof(WeaveItem[j].cbCardData));
				}
				AnalyseItem.cbCardEye=SwitchToCardData(i);
				if(cbCardIndex[i] < 2 || IsMagicCard(i,true))
				{
					AnalyseItem.bMagicEye = true;
				}
				else
				{
					AnalyseItem.bMagicEye = false;
				}

				//插入结果
				AnalyseItemArray.Add(AnalyseItem);

				return true;
			}
		}

		return false;
	}

	//拆分分析
	BYTE cbMagicCardIndex[MAX_INDEX];
	CopyMemory(cbMagicCardIndex,cbCardIndex,sizeof(cbMagicCardIndex));
	//如果有财神
	BYTE cbMagicCardCount = 0;
	if(m_cbMagicIndex != MAX_INDEX)
	{
		//091231JJ
		//花牌财神
		cbMagicCardCount = GetMagicCardCount(cbMagicCardIndex);

		//如果财神有代替牌，财神与代替牌转换
		if(INDEX_REPLACE_CARD != MAX_INDEX)
		{
			cbMagicCardIndex[m_cbMagicIndex] = cbMagicCardIndex[INDEX_REPLACE_CARD];
			cbMagicCardIndex[INDEX_REPLACE_CARD] = cbMagicCardCount;
		}
	}
	if(cbCardCount>=3)
	{
		for(BYTE i=0; i<MAX_INDEX; i++)
		{
			if(i >= MAX_INDEX-MAX_HUA_CARD && i < MAX_INDEX && !IsMagicCard(i,true))
			{
				continue;
			}

			//同牌判断
			//如果是财神,并且财神数小于3,则不进行组合
			if(cbMagicCardIndex[i] >= 3 || (cbMagicCardIndex[i]+cbMagicCardCount >= 3 &&
				((INDEX_REPLACE_CARD!=MAX_INDEX && i != INDEX_REPLACE_CARD) || (INDEX_REPLACE_CARD==MAX_INDEX && !IsMagicCard(i,true))))
				)
			{
				int nTempIndex = cbMagicCardIndex[i];
				do
				{
					ASSERT(cbKindItemCount < CountArray(KindItem));
					BYTE cbIndex = i;
					BYTE cbCenterCard = SwitchToCardData(i);
					//如果是财神且财神有代替牌,则换成代替牌
					if(IsMagicCard(i,true) && INDEX_REPLACE_CARD != MAX_INDEX)
					{
						cbIndex = INDEX_REPLACE_CARD;
						cbCenterCard = SwitchToCardData(INDEX_REPLACE_CARD);
					}
					KindItem[cbKindItemCount].cbWeaveKind=WIK_PENG;
					KindItem[cbKindItemCount].cbCenterCard=cbCenterCard;
					KindItem[cbKindItemCount].cbValidIndex[0] = nTempIndex>0?cbIndex:m_cbMagicIndex;
					KindItem[cbKindItemCount].cbValidIndex[1] = nTempIndex>1?cbIndex:m_cbMagicIndex;
					KindItem[cbKindItemCount].cbValidIndex[2] = nTempIndex>2?cbIndex:m_cbMagicIndex;
					cbKindItemCount++;

					//如果是财神,则退出
					if(i == INDEX_REPLACE_CARD || (IsMagicCard(i,true) && INDEX_REPLACE_CARD == MAX_INDEX))
					{
						break;
					}

					nTempIndex -= 3;
					//如果刚好搭配全部，则退出
					if(nTempIndex == 0)
					{
						break;
					}

				}
				while(nTempIndex+cbMagicCardCount >= 3);
			}

			//连牌判断
			if((i<(MAX_INDEX-MAX_HUA_CARD-9))&&((i%9)<7))
			{
				//只要财神牌数加上3个顺序索引的牌数大于等于3,则进行组合
				if(cbMagicCardCount+cbMagicCardIndex[i]+cbMagicCardIndex[i+1]+cbMagicCardIndex[i+2] >= 3)
				{
					BYTE cbIndex[3] = { cbMagicCardIndex[i],cbMagicCardIndex[i+1],cbMagicCardIndex[i+2] };
					int nMagicCountTemp = cbMagicCardCount;
					BYTE cbValidIndex[3];
					while(nMagicCountTemp+cbIndex[0]+cbIndex[1]+cbIndex[2] >= 3)
					{
						for(BYTE j = 0; j < CountArray(cbIndex); j++)
						{
							if(cbIndex[j] > 0)
							{
								cbIndex[j]--;
								cbValidIndex[j] = (IsMagicCard(i+j,true)&&INDEX_REPLACE_CARD!=MAX_INDEX)?INDEX_REPLACE_CARD:i+j;
							}
							else
							{
								nMagicCountTemp--;
								cbValidIndex[j] = m_cbMagicIndex;
							}
						}
						if(nMagicCountTemp >= 0)
						{
							ASSERT(cbKindItemCount < CountArray(KindItem));
							KindItem[cbKindItemCount].cbWeaveKind=WIK_LEFT;
							KindItem[cbKindItemCount].cbCenterCard=SwitchToCardData(i);
							CopyMemory(KindItem[cbKindItemCount].cbValidIndex,cbValidIndex,sizeof(cbValidIndex));
							cbKindItemCount++;
						}
						else
						{
							break;
						}
					}
				}
			}
		}
	}

	//组合分析
	if(cbKindItemCount>=cbLessKindItem)
	{
		//变量定义
		BYTE cbCardIndexTemp[MAX_INDEX];
		ZeroMemory(cbCardIndexTemp,sizeof(cbCardIndexTemp));

		//变量定义
		BYTE cbIndex[MAX_WEAVE];
		for(BYTE i = 0; i < CountArray(cbIndex); i++)
		{
			cbIndex[i] = i;
		}
		tagKindItem * pKindItem[MAX_WEAVE];
		ZeroMemory(&pKindItem,sizeof(pKindItem));
		tagKindItem KindItemTemp[CountArray(KindItem)];
		if(m_cbMagicIndex != MAX_INDEX)
		{
			CopyMemory(KindItemTemp,KindItem,sizeof(KindItem));
		}

		//开始组合
		do
		{
			//设置变量
			CopyMemory(cbCardIndexTemp,cbCardIndex,sizeof(cbCardIndexTemp));
			cbMagicCardCount = 0;
			if(m_cbMagicIndex != MAX_INDEX)
			{
				CopyMemory(KindItem,KindItemTemp,sizeof(KindItem));
			}

			for(BYTE i=0; i<cbLessKindItem; i++)
			{
				pKindItem[i]=&KindItem[cbIndex[i]];
			}

			//数量判断
			bool bEnoughCard=true;

			for(BYTE i=0; i<cbLessKindItem*3; i++)
			{
				//存在判断
				BYTE cbCardIndex=pKindItem[i/3]->cbValidIndex[i%3];
				if(cbCardIndexTemp[cbCardIndex]==0)
				{
					if(m_cbMagicIndex != MAX_INDEX && GetMagicCardCount(cbCardIndexTemp) > 0)
					{
						for(int k = 0; k < MAX_INDEX; k++)
						{
							if(cbCardIndexTemp[k] > 0 && IsMagicCard(k, true))
							{
								cbCardIndexTemp[k]--;
								pKindItem[i/3]->cbValidIndex[i%3] = k;
							}
						}
					}
					else
					{
						bEnoughCard=false;
						break;
					}
				}
				else
				{
					cbCardIndexTemp[cbCardIndex]--;
				}
			}

			//胡牌判断
			if(bEnoughCard==true)
			{
				//牌眼判断
				BYTE cbCardEye=0;
				bool bMagicEye = false;
				if(GetCardCount(cbCardIndexTemp) == 2)
				{
					for(BYTE i=0; i<MAX_INDEX-MAX_HUA_CARD; i++)
					{
						//091231JJ 胡牌牌眼判断
						if(cbCardIndexTemp[i]==2)
						{
							cbCardEye=SwitchToCardData(i);
							if(m_cbMagicIndex != MAX_INDEX && IsMagicCard(i, true))
							{
								bMagicEye = true;
							}
							break;
						}
						else if(!IsMagicCard(i,true) &&
							m_cbMagicIndex != MAX_INDEX && cbCardIndexTemp[i]+GetMagicCardCount(cbCardIndexTemp)==2)
						{
							cbCardEye = SwitchToCardData(i);
							bMagicEye = true;
							break;
						}
					}
				}

				//组合类型
				if(cbCardEye!=0)
				{
					//变量定义
					tagAnalyseItem AnalyseItem;
					ZeroMemory(&AnalyseItem,sizeof(AnalyseItem));

					//设置组合
					for(BYTE i=0; i<cbWeaveCount; i++)
					{
						AnalyseItem.cbWeaveKind[i]=WeaveItem[i].cbWeaveKind;
						AnalyseItem.cbCenterCard[i]=WeaveItem[i].cbCenterCard;
						GetWeaveCard(WeaveItem[i].cbWeaveKind,WeaveItem[i].cbCenterCard,AnalyseItem.cbCardData[i]);
					}

					//设置牌型
					for(BYTE i=0; i<cbLessKindItem; i++)
					{
						AnalyseItem.cbWeaveKind[i+cbWeaveCount]=pKindItem[i]->cbWeaveKind;
						AnalyseItem.cbCenterCard[i+cbWeaveCount]=pKindItem[i]->cbCenterCard;
						AnalyseItem.cbCardData[cbWeaveCount+i][0] = SwitchToCardData(pKindItem[i]->cbValidIndex[0]);
						AnalyseItem.cbCardData[cbWeaveCount+i][1] = SwitchToCardData(pKindItem[i]->cbValidIndex[1]);
						AnalyseItem.cbCardData[cbWeaveCount+i][2] = SwitchToCardData(pKindItem[i]->cbValidIndex[2]);
					}

					//设置牌眼
					AnalyseItem.cbCardEye=cbCardEye;
					AnalyseItem.bMagicEye = bMagicEye;

					//插入结果
					AnalyseItemArray.Add(AnalyseItem);
				}
			}

			//设置索引
			if(cbIndex[cbLessKindItem-1]==(cbKindItemCount-1))
			{
				BYTE i=cbLessKindItem-1;
				for(; i>0; i--)
				{
					if((cbIndex[i-1]+1)!=cbIndex[i])
					{
						BYTE cbNewIndex=cbIndex[i-1];
						for(BYTE j=(i-1); j<cbLessKindItem; j++)
						{
							cbIndex[j]=cbNewIndex+j-i+2;
						}
						break;
					}
				}
				if(i==0)
				{
					break;
				}
			}
			else
			{
				cbIndex[cbLessKindItem-1]++;
			}
		}
		while(true);

	}

	return (AnalyseItemArray.GetCount()>0);
}

//花牌判断
bool CGameLogic::IsHuaCard(BYTE cbCardData)
{
	ASSERT(IsValidCard(cbCardData));

	return cbCardData>=0x38?true:false;
}

//脱牌判断
bool CGameLogic::IsTuoPai(BYTE cbOutCard, tagWeaveItem * pWeaveItem)
{
	ASSERT(pWeaveItem->cbWeaveKind&(WIK_LEFT|WIK_CENTER|WIK_RIGHT));

	if(cbOutCard == pWeaveItem->cbCenterCard)
	{
		return true;
	}

	if(pWeaveItem->cbWeaveKind == WIK_LEFT)
	{
		if(cbOutCard == pWeaveItem->cbCenterCard+3)
		{
			return true;
		}
	}
	else if(pWeaveItem->cbWeaveKind == WIK_RIGHT)
	{
		if(cbOutCard == pWeaveItem->cbCenterCard-3)
		{
			return true;
		}
	}

	return false;
}

//排序,根据牌值排序
bool CGameLogic::SortCardList(BYTE cbCardData[MAX_COUNT], BYTE cbCardCount)
{
	//数目过虑
	if(cbCardCount==0||cbCardCount>MAX_COUNT)
	{
		return false;
	}

	//排序操作
	bool bSorted=true;
	BYTE cbSwitchData=0,cbLast=cbCardCount-1;
	do
	{
		bSorted=true;
		for(BYTE i=0; i<cbLast; i++)
		{
			if(cbCardData[i]>cbCardData[i+1])
			{
				//设置标志
				bSorted=false;

				//扑克数据
				cbSwitchData=cbCardData[i];
				cbCardData[i]=cbCardData[i+1];
				cbCardData[i+1]=cbSwitchData;
			}
		}
		cbLast--;
	}
	while(bSorted==false);

	return true;
}

void CGameLogic::AnalyseZiCard(BYTE cbCardIndex[MAX_INDEX], BYTE cbZiCardData[7], BYTE &cbZiCardCount)
{
	for(WORD i=0; i<MAX_INDEX; i++)
	{
		if(cbCardIndex[i] == 0)
		{
			continue;
		}

		if(cbCardIndex[i] == 1 && i >= 27 && i <= 33)
		{
			cbZiCardData[cbZiCardCount++] = SwitchToCardData(i);
		}
	}
}

//分析风牌和箭牌张数
void CGameLogic::AnalyseFengAndJianCardCount(BYTE cbCardData[7], BYTE &cbFengCardCount, BYTE &cbJianCardCount)
{
	for(WORD i=0; i<7; i++)
	{
		if(cbCardData[i] == 0)
		{
			continue;
		}

		if(SwitchToCardIndex(cbCardData[i]) >= 27 && SwitchToCardIndex(cbCardData[i]) <= 30)
		{
			cbFengCardCount++;
		}
		else if(SwitchToCardIndex(cbCardData[i]) >= 31 && SwitchToCardIndex(cbCardData[i]) <= 33)
		{
			cbJianCardCount++;
		}
	}
}

bool CGameLogic::IsFengCard(BYTE cbCardData)
{
	return (SwitchToCardIndex(cbCardData) >= 27 && SwitchToCardIndex(cbCardData) <= 30);
}

bool CGameLogic::IsJianCard(BYTE cbCardData)
{
	return (SwitchToCardIndex(cbCardData) >= 31 && SwitchToCardIndex(cbCardData) <= 33);
}

/*
// 胡法分析函数
*/

//单吊
bool CGameLogic::IsDanDiaoJiang(const tagAnalyseItem *pAnalyseItem, BYTE cbCurrentCard)
{
	if(pAnalyseItem->cbCardEye==cbCurrentCard || (pAnalyseItem->bMagicEye&&IsMagicCard(cbCurrentCard, false)))
	{
		return true;
	}
	return false;
}

//八对
bool CGameLogic::IsBaDui(const BYTE cbCardIndex[MAX_INDEX], const tagWeaveItem WeaveItem[], const BYTE cbWeaveCount, bool & bYingBaDui)
{
	//组合判断
	if(cbWeaveCount!=0)
	{
		return false;
	}

	//变量定义
	BYTE cbDoubleCount = 0;

	//临时数据
	BYTE cbCardIndexTemp[MAX_INDEX];
	CopyMemory(cbCardIndexTemp,cbCardIndex,sizeof(cbCardIndexTemp));

	//计算对牌
	for(BYTE i=0; i<MAX_INDEX; i++)
	{
		BYTE cbCardCount=cbCardIndexTemp[i];

		//王牌过滤
		if(i == m_cbMagicIndex)
		{
			continue;
		}

		//单牌统计
		if(cbCardCount == 2 || cbCardCount == 3)
		{
			cbDoubleCount++;
		}
	}

	//对牌足够
	if(cbDoubleCount == 8)
	{
		bYingBaDui = true;
		return true;
	}

	//王牌不够
	if((m_cbMagicIndex != MAX_INDEX && 8-cbDoubleCount > cbCardIndexTemp[m_cbMagicIndex]) ||
		(m_cbMagicIndex == MAX_INDEX && 8-cbDoubleCount > 0))
	{
		return false;
	}

	if(cbDoubleCount + cbCardIndex[m_cbMagicIndex]/2 == 8)
	{
		bYingBaDui = true;
	}
	else
	{
		bYingBaDui = false;
	}

	return true;
}

//碰碰胡
bool CGameLogic::IsPengPeng(const tagAnalyseItem *pAnalyseItem)
{
	for(BYTE i = 0; i < CountArray(pAnalyseItem->cbWeaveKind); i++)
	{
		if(pAnalyseItem->cbWeaveKind[i]&(WIK_LEFT|WIK_CENTER|WIK_RIGHT))
		{
			return false;
		}
	}
	return true;
}

//硬胡
bool CGameLogic::IsYingHu(const tagAnalyseItem *pAnalyseItem, BYTE cbWeaveCount)
{
	//无财神不算
	if(m_cbMagicIndex == MAX_INDEX)
	{
		return false;
	}

	//已吃碰出的组合不算
	for(BYTE i = cbWeaveCount; i < CountArray(pAnalyseItem->cbWeaveKind); i++)
	{
		//检查是否有财神
		for(int j = 0; j < 3; j++)
		{
			if(pAnalyseItem->cbCardData[i][j] == SwitchToCardData(m_cbMagicIndex))
			{
				//检查还搭
				BYTE bTempCardData[4] ;
				ZeroMemory(bTempCardData,sizeof(bTempCardData));
				GetWeaveCard(pAnalyseItem->cbWeaveKind[i], pAnalyseItem->cbCenterCard[i], bTempCardData);

				if(bTempCardData[j] == SwitchToCardData(m_cbMagicIndex))
				{
					continue;
				}
				else
				{
					return false;
				}
			}
		}
	}

	//检查牌眼
	if(pAnalyseItem->bMagicEye && pAnalyseItem->cbCardEye != SwitchToCardData(m_cbMagicIndex))
	{
		return false;
	}

	return true;
}

//三金倒
bool CGameLogic::IsSanJinDao(const BYTE cbCardIndex[MAX_INDEX])
{
	//无财神不算
	if(m_cbMagicIndex == MAX_INDEX)
	{
		return false;
	}

	BYTE bMagicCardCount = GetMagicCardCount(cbCardIndex);

	if(bMagicCardCount >= 3)
	{
		return true;
	}
	else
	{
		return false;
	}
}
//////////////////////////////////////////////////////////////////////////
