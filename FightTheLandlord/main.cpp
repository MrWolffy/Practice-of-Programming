// 斗地主（FightTheLandlord）样例程序
// 无脑策略
// 最后更新于2018-5-8
// 作者：zhouhy
// 游戏信息：http://www.botzone.org/games#FightTheLandlord

#include <iostream>
#include <set>
#include <string>
#include <cassert>
#include <cstring> // 注意memset是cstring里的
#include <algorithm>
#include <map>
#include "jsoncpp/json.h" // 在平台上，C++编译时默认包含此库

using std::vector;
using std::sort;
using std::unique;
using std::set;
using std::string;
using std::cout;
using std::endl;
using std::pair;

constexpr int PLAYER_COUNT = 3;

enum class CardComboType
{
    PASS, // 过
    SINGLE, // 单张
    PAIR, // 对子
    STRAIGHT, // 顺子
    STRAIGHT2, // 双顺
    TRIPLET, // 三条
    TRIPLET1, // 三带一
    TRIPLET2, // 三带二
    BOMB, // 炸弹
    QUADRUPLE2, // 四带二（只）
    QUADRUPLE4, // 四带二（对）
    PLANE, // 飞机
    PLANE1, // 飞机带小翼
    PLANE2, // 飞机带大翼
    SSHUTTLE, // 航天飞机
    SSHUTTLE2, // 航天飞机带小翼
    SSHUTTLE4, // 航天飞机带大翼
    ROCKET, // 火箭
    INVALID // 非法牌型
};

int cardComboScores[] = {
        0, // 过
        1, // 单张
        2, // 对子
        6, // 顺子
        6, // 双顺
        4, // 三条
        4, // 三带一
        4, // 三带二
        10, // 炸弹
        8, // 四带二（只）
        8, // 四带二（对）
        8, // 飞机
        8, // 飞机带小翼
        8, // 飞机带大翼
        10, // 航天飞机（需要特判：二连为10分，多连为20分）
        10, // 航天飞机带小翼
        10, // 航天飞机带大翼
        16, // 火箭
        0 // 非法牌型
};

#ifndef _BOTZONE_ONLINE
string cardComboStrings[] = {
        "PASS",
        "SINGLE",
        "PAIR",
        "STRAIGHT",
        "STRAIGHT2",
        "TRIPLET",
        "TRIPLET1",
        "TRIPLET2",
        "BOMB",
        "QUADRUPLE2",
        "QUADRUPLE4",
        "PLANE",
        "PLANE1",
        "PLANE2",
        "SSHUTTLE",
        "SSHUTTLE2",
        "SSHUTTLE4",
        "ROCKET",
        "INVALID"
};
#endif

// 用0~53这54个整数表示唯一的一张牌
using Card = short;
constexpr Card card_joker = 52;
constexpr Card card_JOKER = 53;

// 除了用0~53这54个整数表示唯一的牌，
// 这里还用另一种序号表示牌的大小（不管花色），以便比较，称作等级（Level）
// 对应关系如下：
// 3 4 5 6 7 8 9 10	J Q K	A	2	小王	大王
// 0 1 2 3 4 5 6 7	8 9 10	11	12	13	14
using Level = short;
constexpr Level MAX_LEVEL = 15;
constexpr Level MAX_STRAIGHT_LEVEL = 11;
constexpr Level level_joker = 13;
constexpr Level level_JOKER = 14;

// 我的牌有哪些
set<Card> myCards;

// 地主被明示的牌有哪些
set<Card> landlordPublicCards;

// 大家从最开始到现在都出过什么
vector<vector<Card>> whatTheyPlayed[PLAYER_COUNT];

// 大家还剩多少牌
short cardRemaining[PLAYER_COUNT] = { 20, 17, 17 };

// 我是几号玩家（0-地主，1-农民甲，2-农民乙）
int myPosition;



/**
 * 将Card变成Level
 */
constexpr Level card2level(Card card)
{
    return card / 4 + card / 53;
}

// 牌的组合，用于计算牌型
struct CardCombo
{
    // 表示同等级的牌有多少张
    // 会按个数从大到小、等级从大到小排序
    struct CardPack
    {
        Level level;
        short count;

        bool operator< (const CardPack& b) const
        {
            if (count == b.count)
                return level > b.level;
            return count > b.count;
        }
        // 增加CardPack的输出函数以供调试
        // by尹晨桥 5.15
        friend std::ostream & operator<< (std::ostream &os, CardPack &p) {
            os << "(" << p.level << "," << p.count << ") ";
            return os;
        }
        // end
    };
    vector<Card> cards; // 原始的牌，未排序
    vector<CardPack> packs; // 按数目和大小排序的牌种
    CardComboType comboType; // 算出的牌型
    Level comboLevel = 0; // 算出的大小序

    /**
     * 检查个数最多的CardPack递减了几个
     */
    int findMaxSeq() const
    {
        for (unsigned c = 1; c < packs.size(); c++)
            if (packs[c].count != packs[0].count ||
                packs[c].level != packs[c - 1].level - 1)
                return c;
        return packs.size();
    }

    /**
     * 这个牌型最后算总分的时候的权重
     */
    int getWeight() const
    {
        if (comboType == CardComboType::SSHUTTLE ||
            comboType == CardComboType::SSHUTTLE2 ||
            comboType == CardComboType::SSHUTTLE4)
            return cardComboScores[(int)comboType] + (findMaxSeq() > 2) * 10;
        return cardComboScores[(int)comboType];
    }

    // 创建一个空牌组
    CardCombo() : comboType(CardComboType::PASS) {}

    /**
     * 通过Card（即short）类型的迭代器创建一个牌型
     * 并计算出牌型和大小序等
     * 假设输入没有重复数字（即重复的Card）
     */
    template <typename CARD_ITERATOR>
    CardCombo(CARD_ITERATOR begin, CARD_ITERATOR end)
    {
        // 特判：空
        if (begin == end)
        {
            comboType = CardComboType::PASS;
            return;
        }

        // 每种牌有多少个
        short counts[MAX_LEVEL + 1] = {};

        // 同种牌的张数（有多少个单张、对子、三条、四条）
        short countOfCount[5] = {};

        cards = vector<Card>(begin, end);
        for (Card c : cards)
            counts[card2level(c)]++;
        for (Level l = 0; l <= MAX_LEVEL; l++)
            if (counts[l])
            {
                packs.push_back(CardPack{ l, counts[l] });
                countOfCount[counts[l]]++;
            }
        sort(packs.begin(), packs.end());

        // 用最多的那种牌总是可以比较大小的
        comboLevel = packs[0].level;

        // 计算牌型
        // 按照 同种牌的张数 有几种 进行分类
        vector<int> kindOfCountOfCount;
        for (int i = 0; i <= 4; i++)
            if (countOfCount[i])
                kindOfCountOfCount.push_back(i);
        sort(kindOfCountOfCount.begin(), kindOfCountOfCount.end());

        int curr, lesser;

        switch (kindOfCountOfCount.size())
        {
            case 1: // 只有一类牌
                curr = countOfCount[kindOfCountOfCount[0]];
                switch (kindOfCountOfCount[0])
                {
                    case 1:
                        // 只有若干单张
                        if (curr == 1)
                        {
                            comboType = CardComboType::SINGLE;
                            return;
                        }
                        if (curr == 2 && packs[1].level == level_joker)
                        {
                            comboType = CardComboType::ROCKET;
                            return;
                        }
                        if (curr >= 5 && findMaxSeq() == curr &&
                            packs.begin()->level <= MAX_STRAIGHT_LEVEL)
                        {
                            comboType = CardComboType::STRAIGHT;
                            return;
                        }
                        break;
                    case 2:
                        // 只有若干对子
                        if (curr == 1)
                        {
                            comboType = CardComboType::PAIR;
                            return;
                        }
                        if (curr >= 3 && findMaxSeq() == curr &&
                            packs.begin()->level <= MAX_STRAIGHT_LEVEL)
                        {
                            comboType = CardComboType::STRAIGHT2;
                            return;
                        }
                        break;
                    case 3:
                        // 只有若干三条
                        if (curr == 1)
                        {
                            comboType = CardComboType::TRIPLET;
                            return;
                        }
                        if (findMaxSeq() == curr &&
                            packs.begin()->level <= MAX_STRAIGHT_LEVEL)
                        {
                            comboType = CardComboType::PLANE;
                            return;
                        }
                        break;
                    case 4:
                        // 只有若干四条
                        if (curr == 1)
                        {
                            comboType = CardComboType::BOMB;
                            return;
                        }
                        if (findMaxSeq() == curr &&
                            packs.begin()->level <= MAX_STRAIGHT_LEVEL)
                        {
                            comboType = CardComboType::SSHUTTLE;
                            return;
                        }
                }
                break;
            case 2: // 有两类牌
                curr = countOfCount[kindOfCountOfCount[1]];
                lesser = countOfCount[kindOfCountOfCount[0]];
                if (kindOfCountOfCount[1] == 3)
                {
                    // 三条带？
                    if (kindOfCountOfCount[0] == 1)
                    {
                        // 三带一
                        if (curr == 1 && lesser == 1)
                        {
                            comboType = CardComboType::TRIPLET1;
                            return;
                        }
                        if (findMaxSeq() == curr && lesser == curr &&
                            packs.begin()->level <= MAX_STRAIGHT_LEVEL)
                        {
                            comboType = CardComboType::PLANE1;
                            return;
                        }
                    }
                    if (kindOfCountOfCount[0] == 2)
                    {
                        // 三带二
                        if (curr == 1 && lesser == 1)
                        {
                            comboType = CardComboType::TRIPLET2;
                            return;
                        }
                        if (findMaxSeq() == curr && lesser == curr &&
                            packs.begin()->level <= MAX_STRAIGHT_LEVEL)
                        {
                            comboType = CardComboType::PLANE2;
                            return;
                        }
                    }
                }
                if (kindOfCountOfCount[1] == 4)
                {
                    // 四条带？
                    if (kindOfCountOfCount[0] == 1)
                    {
                        // 四条带两只 * n
                        if (curr == 1 && lesser == 2)
                        {
                            comboType = CardComboType::QUADRUPLE2;
                            return;
                        }
                        if (findMaxSeq() == curr && lesser == curr * 2 &&
                            packs.begin()->level <= MAX_STRAIGHT_LEVEL)
                        {
                            comboType = CardComboType::SSHUTTLE2;
                            return;
                        }
                    }
                    if (kindOfCountOfCount[0] == 2)
                    {
                        // 四条带两对 * n
                        if (curr == 1 && lesser == 2)
                        {
                            comboType = CardComboType::QUADRUPLE4;
                            return;
                        }
                        if (findMaxSeq() == curr && lesser == curr * 2 &&
                            packs.begin()->level <= MAX_STRAIGHT_LEVEL)
                        {
                            comboType = CardComboType::SSHUTTLE4;
                            return;
                        }
                    }
                }
        }

        comboType = CardComboType::INVALID;
    }

    /**
     * 判断指定牌组能否大过当前牌组（这个函数不考虑过牌的情况！）
     */
    bool canBeBeatenBy(const CardCombo& b) const
    {
        if (comboType == CardComboType::INVALID || b.comboType == CardComboType::INVALID)
            return false;
        if (b.comboType == CardComboType::ROCKET)
            return true;
        if (b.comboType == CardComboType::BOMB)
            switch (comboType)
            {
                case CardComboType::ROCKET:
                    return false;
                case CardComboType::BOMB:
                    return b.comboLevel > comboLevel;
                default:
                    return true;
            }
        return b.comboType == comboType && b.cards.size() == cards.size() && b.comboLevel > comboLevel;
    }

    // 寻找最小的不需要拆牌的单张
    // by尹晨桥 5.15
    template <typename CARD_ITERATOR>
    pair<bool, CardCombo> findFirstSingle(CARD_ITERATOR begin, CARD_ITERATOR end) {
        auto deck = vector<Card>(begin, end); // 手牌
        short counts[MAX_LEVEL + 1] = {};
        for (Card c : deck)
            counts[card2level(c)]++;

        int firstvalid = 0;
        for (; firstvalid <= MAX_LEVEL; ++firstvalid) {
            if (counts[firstvalid] == 1) break;
        }
        if (firstvalid == MAX_LEVEL + 1) return std::make_pair(false, CardCombo());

        vector<Card> solve;
        for (Card c : deck)
        {
            Level level = card2level(c);
            if (level == firstvalid)
            {
                solve.push_back(c);
                break;
            }
        }
        return std::make_pair(true, CardCombo(solve.begin(), solve.end()));
    };

    // 寻找最小的不需要拆牌的对子
    // by尹晨桥 5.15
    template <typename CARD_ITERATOR>
    pair<bool, CardCombo> findFirstPair(CARD_ITERATOR begin, CARD_ITERATOR end) {
        auto deck = vector<Card>(begin, end); // 手牌
        short counts[MAX_LEVEL + 1] = {};
        for (Card c : deck)
            counts[card2level(c)]++;

        /*for (int i = 0; i < MAX_LEVEL; ++i) {
            cout << counts[i] << " ";
        }
        cout << endl;*/

        int firstvalid = 0;
        for (; firstvalid < level_joker; ++firstvalid) {
            if (counts[firstvalid] == 2) break;
        }
        if (firstvalid == level_joker) return std::make_pair(false, CardCombo());

        vector<Card> solve;
        int need = 2;
        for (Card c : deck)
        {
            Level level = card2level(c);
            if (level == firstvalid)
            {
                solve.push_back(c);
                need--;
                if (need == 0) break;
            }
        }
        return std::make_pair(true, CardCombo(solve.begin(), solve.end()));
    };

    // 最简单的拆牌算法的返回值
    // by尹晨桥 5.17
    struct levels {
        vector<Level> single;
        vector<Level> pair;
        vector<Level> triplet;
        vector<Level> bomb;
        int countEachLevel[MAX_LEVEL + 1];
        short total_number;
    };

    // 增加最简单的拆牌算法，将每一个level的牌作为一类
    // by尹晨桥 5.17
    template <typename CARD_ITERATOR>
    levels levelCount_naive (CARD_ITERATOR begin, CARD_ITERATOR end) {
        auto deck = vector<Card>(begin, end); // 手牌
        short counts[MAX_LEVEL + 1] = {0};
        memset(counts, 0, sizeof(counts));
        for (Card c : deck)
            counts[card2level(c)]++;

        levels tmp;
        memset(tmp.countEachLevel, 0, sizeof(tmp.countEachLevel));
        for (int i = 0; i <= MAX_LEVEL; ++i) tmp.countEachLevel[i] = counts[i];
        short level_iterator = 0;
        for (; level_iterator < MAX_LEVEL; ++level_iterator) {
            switch (counts[level_iterator]) {
                case 1: {
                    tmp.single.push_back(level_iterator);
                    break;
                }
                case 2: {
                    tmp.pair.push_back(level_iterator);
                    break;
                }
                case 3: {
                    tmp.triplet.push_back(level_iterator);
                    break;
                }
                case 4: {
                    tmp.bomb.push_back(level_iterator);
                    break;
                }
                default: break;
            }
        }
        /*cout << "single: ";
        for (auto i = tmp.single.begin(); i != tmp.single.end(); ++i)
            cout << *i << " ";
        cout << endl;
        cout << "pair: ";
        for (auto i = tmp.pair.begin(); i != tmp.pair.end(); ++i)
            cout << *i << " ";
        cout << endl;
        cout << "triplet: ";
        for (auto i = tmp.triplet.begin(); i != tmp.triplet.end(); ++i)
            cout << *i << " ";
        cout << endl;
        cout << "bomb: ";
        for (auto i = tmp.bomb.begin(); i != tmp.bomb.end(); ++i)
            cout << *i << " ";
        cout << endl;
        for (int i = 0; i <= MAX_LEVEL; ++i) {
            cout << "level: " << i << ", count: " << counts[i] << endl;
        }*/

        //补加总数
        //by 付云天 5.19
        tmp.total_number = deck.size();
        return tmp;
    }

    // 增加从level产生牌组的函数
    // by尹晨桥 5.19
    CardCombo makeCardComboFromLevel (Level l, int count) {
        vector<Card> tmp;
        for (auto i = myCards.begin(); i != myCards.end() && count > 0; ++i) {
            if (card2level(*i) == l) {
                tmp.push_back(*i);
                --count;
            }
        }
        return CardCombo(tmp.begin(), tmp.end());
    }

    //重载makeCardComboFromLevel
    //by 付云天 5.19
    vector<Card> makeCardComboFromLevel (vector<Card>a) {
        sort(a.begin(), a.end());
        vector<Card> tmp;
        vector<Card>::iterator iter = a.begin();
        for (auto i = myCards.begin(); i != myCards.end()&&iter != a.end(); ++i) {
            if (card2level(*i) == *iter) {
                tmp.push_back(*i);
                iter++;
            }
        }
        return tmp;
    }



    // 主动出牌的函数
    template <typename CARD_ITERATOR>
    CardCombo findFirstValid_ifPass (CARD_ITERATOR begin, CARD_ITERATOR end) {

        // 增加最简单的拆牌算法
        // by尹晨桥 5.15
        auto levelCount = levelCount_naive(begin, end);
        CardCombo nowCombo = CardCombo(begin, end);
        // 修改随便出的算法，如果剩余的牌能凑成一种牌型，就全部出掉
        // by尹晨桥 5.15
        if (nowCombo.comboType != CardComboType::INVALID) return nowCombo;


        // 寻找高级牌, 并记录张数
        //by付云天 5.19
        bool flag = 1;
        if (levelCount.total_number >= 5 && flag) {
            //寻找飞机
            vector < Level>plane;
            short number3 = levelCount.triplet.size();
            if (number3 >= 2) {
                for (short i = number3; i >= 2; i++) {
                    for (short j = 0; j < number3 - i + 1; j++) {
                        if (levelCount.triplet[j + i - 1] - levelCount.triplet[j] == i - 1 && levelCount.triplet[j + i - 1] != 12) {
                            for (short k = j; k < j + i; k++) {
                                plane.push_back(levelCount.triplet[k]);
                            }
                            break;
                        }
                    }
                }
            }
            //暴力拆双顺
            vector<Level>straight2;
            short number2 = levelCount.pair.size() + levelCount.triplet.size();
            vector<Level>PairPlusTriplet = levelCount.pair;
            PairPlusTriplet.insert(PairPlusTriplet.end(),
                                   levelCount.triplet.begin(), levelCount.triplet.end());
            sort(PairPlusTriplet.begin(), PairPlusTriplet.end());
            if (number2 >= 3) {
                for (short i = number2; i >= 3; i--) {
                    for (short j = 0; j < number2 - i + 1; j++) {
                        if (PairPlusTriplet[j + i - 1] - PairPlusTriplet[j] == i - 1
                            && PairPlusTriplet[j + i - 1] < 12) {
                            for (short k = j; k < j + i; k++) {
                                straight2.push_back(PairPlusTriplet[k]);
                            }
                            break;
                        }
                    }
                }
            }
            //暴力拆顺子
            vector<Level>straight;
            vector<Level>SinglePlusPairPlusTriplet = PairPlusTriplet;
            SinglePlusPairPlusTriplet.insert(SinglePlusPairPlusTriplet.end(),
                                             levelCount.single.begin(), levelCount.single.end());
            short number = SinglePlusPairPlusTriplet.size();
            sort(SinglePlusPairPlusTriplet.begin(), SinglePlusPairPlusTriplet.end());
            if (number >= 5) {
                for (short i = number; i >= 5; i--) {
                    for (short j = 0; j < number - i + 1; j++) {
                        if (SinglePlusPairPlusTriplet[j + i - 1] - SinglePlusPairPlusTriplet[j] == i - 1
                            && SinglePlusPairPlusTriplet[j + i - 1] < 12) {
                            for (short k = j; k < j + i; k++) {
                                straight.push_back(SinglePlusPairPlusTriplet[k]);
                            }
                            break;
                        }
                    }
                }
            }


            //判断如何出高级牌
            if (straight.size() || straight2.size() || plane.size()) {
                //判断飞机是否可出
                number2 = levelCount.pair.size();
                number = levelCount.single.size();
                if (plane.size()) {
                    if (*(plane.rbegin()) < 10 || *(plane.rbegin()) > 10 || levelCount.total_number < 12) {
                        //判断是否带大机翼
                        if (number2 >= plane.size()) {
                            vector<Card> wing;
                            //若飞机为二连，则不拆双顺
                            if (plane.size() == 2) {
                                for (short i = 0; i < number2; i++) {
                                    if (find(straight2.begin(), straight2.end(), levelCount.pair[i]) == straight2.end()) {
                                        wing.push_back(levelCount.pair[i]);
                                    }
                                }
                                //若大机翼足够
                                if (wing.size() >= plane.size()) {
                                    vector<Card> solve;
                                    for (int i = 0; i < plane.size(); i++) {
                                        for (int j = 0; j < 3; j++) {
                                            solve.push_back(plane[i]);
                                        }
                                    }
                                    for (int i = 0; i < plane.size(); i++) {
                                        for (int j = 0; j < 2; j++) {
                                            solve.push_back(wing[i]);
                                        }
                                    }
                                    solve = makeCardComboFromLevel(solve);
                                    return CardCombo(solve.begin(), solve.end());
                                }
                                    //若大机翼不够，则尝试小机翼
                                else {
                                    wing.clear();
                                    for (short i = 0; i < number; i++) {
                                        if (find(straight.begin(), straight.end(), levelCount.single[i]) == straight.end()) {
                                            wing.push_back(levelCount.single[i]);
                                        }
                                    }
                                    if (wing.size() >= plane.size()) {
                                        vector<Card> solve;
                                        for (int i = 0; i < plane.size(); i++) {
                                            for (int j = 0; j < 3; j++) {
                                                solve.push_back(plane[i]);
                                            }
                                        }
                                        for (int i = 0; i < plane.size(); i++) {
                                            solve.push_back(wing[i]);
                                        }
                                        solve = makeCardComboFromLevel(solve);
                                        return CardCombo(solve.begin(), solve.end());
                                    }
                                    else {
                                        vector<Card> solve;
                                        for (int i = 0; i < plane.size(); i++) {
                                            for (int j = 0; j < 3; j++) {
                                                solve.push_back(plane[i]);
                                            }
                                        }
                                        solve = makeCardComboFromLevel(solve);
                                        return CardCombo(solve.begin(), solve.end());
                                    }
                                }
                                //若小机翼也不够，则不带机翼

                            }
                            else if (plane.size() > 2) {
                                //如果对子数足够则带大机翼
                                if (number2 >= plane.size()) {
                                    for (short i = 0; i < number2; i++) {
                                        wing.push_back(levelCount.pair[i]);
                                    }
                                    vector<Card> solve;
                                    for (int i = 0; i < plane.size(); i++) {
                                        for (int j = 0; j < 3; j++) {
                                            solve.push_back(plane[i]);
                                        }
                                    }
                                    for (int i = 0; i < plane.size(); i++) {
                                        for (int j = 0; j < 2; j++) {
                                            solve.push_back(wing[i]);
                                        }
                                    }
                                    solve = makeCardComboFromLevel(solve);
                                    return CardCombo(solve.begin(), solve.end());
                                }
                                    //对子数不够则尝试小机翼
                                else {
                                    if (number > plane.size()) {
                                        for (short i = 0; i < number; i++) {
                                            wing.push_back(levelCount.single[i]);
                                        }
                                        vector<Card> solve;
                                        for (int i = 0; i < plane.size(); i++) {
                                            for (int j = 0; j < 3; j++) {
                                                solve.push_back(plane[i]);
                                            }
                                        }
                                        for (int i = 0; i < plane.size(); i++) {
                                            solve.push_back(wing[i]);
                                        }
                                        solve = makeCardComboFromLevel(solve);
                                        return CardCombo(solve.begin(), solve.end());
                                    }
                                        //都不够则不带机翼
                                    else {
                                        vector<Card> solve;
                                        for (int i = 0; i < plane.size(); i++) {
                                            for (int j = 0; j < 3; j++) {
                                                solve.push_back(plane[i]);
                                            }
                                        }
                                        solve = makeCardComboFromLevel(solve);
                                        return CardCombo(solve.begin(), solve.end());
                                    }
                                }


                            }
                        }

                    }
                }
                //根据单顺和双顺的张数来出牌，牌数相差3以上则出牌数多的
                if (straight.size() - straight2.size() >= 3|| straight.size() - straight2.size()<=-3) {
                    if (straight.size() > straight2.size()) {
                        vector<Card>solve;
                        for (int i = 0; i < straight.size(); i++) {
                            solve.push_back(straight[i]);
                        }
                        solve = makeCardComboFromLevel(solve);
                        return CardCombo(solve.begin(), solve.end());
                    }
                    else {
                        if (*(straight2.rbegin()) <= 11 || levelCount.total_number < 10) {
                            vector<Card>solve;
                            for (int i = 0; i < straight2.size(); i++) {
                                for (int j = 0; j < 2; j++) {
                                    solve.push_back(straight2[i]);
                                }
                            }
                            solve = makeCardComboFromLevel(solve);
                            return CardCombo(solve.begin(), solve.end());
                        }
                            //如果因双顺过大不出，则考虑单顺
                        else if (straight.size()) {
                            vector<Card>solve;
                            for (int i = 0; i < straight.size(); i++) {
                                solve.push_back(straight[i]);
                            }
                            solve = makeCardComboFromLevel(solve);
                            return CardCombo(solve.begin(), solve.end());
                        }
                    }
                }
                    //牌数相差较小则出小的
                else  {
                    if (*(straight2.rbegin()) < *(straight.rbegin())) {
                        vector<Card>solve;
                        for (int i = 0; i < straight2.size(); i++) {
                            for (int j = 0; j < 2; j++) {
                                solve.push_back(straight2[i]);
                            }
                        }
                        solve = makeCardComboFromLevel(solve);
                        return CardCombo(solve.begin(), solve.end());
                    }
                    else {
                        vector<Card>solve;
                        for (int i = 0; i < straight.size(); i++) {
                            solve.push_back(straight[i]);
                        }
                        solve = makeCardComboFromLevel(solve);
                        return CardCombo(solve.begin(), solve.end());
                    }
                }
            }
            else {
                //如果搜不出高级牌，之后也不可能有高级牌，不再搜索
                flag = 0;
            }
        }

        // 修改随便出的算法，如果存在不需要拆牌的单张和对子，就出一个最小的单张或对子
        // by尹晨桥 5.15
        auto Single = findFirstSingle(begin, end);
        auto Pair = findFirstPair(begin, end);

        // 考虑三带
        if (levelCount.triplet.size()) {
            //若三带较大，则留到牌少的时候应急
            if (*(levelCount.triplet.begin()) < 10 || levelCount.total_number < 6) {
                vector<Card>solve;
                //考虑三代双
                if (Pair.first) {
                    if (*(Pair.second.cards.begin()) < 41 || levelCount.total_number < 6)
                        for (int i = 0; i < 3; i++) {
                            solve.push_back(levelCount.triplet[0]);
                        }
                    solve = makeCardComboFromLevel(solve);
                    solve.push_back(*(Pair.second.cards.begin()));
                    solve.push_back(*(++Pair.second.cards.begin()));
                    return CardCombo(solve.begin(), solve.end());
                }
                //不行就三带一
                if (Single.first) {
                    for (int i = 0; i < 3; i++) {
                        solve.push_back(levelCount.triplet[0]);
                    }
                    solve = makeCardComboFromLevel(solve);
                    solve.push_back(*(Single.second.cards.begin()));
                    return CardCombo(solve.begin(), solve.end());
                }
                //还不行就三不带
                {
                    for (int i = 0; i < 3; i++) {
                        solve.push_back(levelCount.triplet[0]);
                    }
                    solve = makeCardComboFromLevel(solve);
                    return CardCombo(solve.begin(), solve.end());
                }
            }
        }

        // 都打不出就打单张或对子
        // 如果对方只剩一张牌
        if ((myPosition && cardRemaining[0] == 1) ||
                (!myPosition && (cardRemaining[1] == 1 || cardRemaining[2] == 1))) {

            // 有对先出对
            if (Pair.first) return Pair.second;

            // 没对从大往小出单牌
            if (!levelCount.single.empty())
                return makeCardComboFromLevel(*(--levelCount.single.end()), 1);
        }
        // 如果对方只剩两张牌
        if ((myPosition && cardRemaining[0] == 2) ||
            (!myPosition && (cardRemaining[1] == 2 || cardRemaining[2] == 2))) {

            // 有单先出单
            if (Single.first) return Single.second;

            // 没单从大往小出对子
            if (!levelCount.pair.empty())
                return makeCardComboFromLevel(*(--levelCount.pair.end()), 2);
        }

        // 如果还没到最后残局，就出较小的一个
        if (Single.first == true && Pair.first == false) {
            //cout << "have single" << endl;
            return Single.second;
        }
        if (Single.first == false && Pair.first == true) {
            //cout << "have pair" << endl;
            return Pair.second;
        }
        if (Single.first == true && Pair.first == true) {
            //cout << "have single and pair" << endl;
            //cout << "single: " << *Single.second.cards.begin() << endl;
            //cout << "pair: " << *Pair.second.cards.begin() << endl;

            if (*Single.second.cards.begin() < *Pair.second.cards.begin()
                || *Single.second.cards.rbegin() >= 44)
                return Single.second;
            return Pair.second;
        }


        CARD_ITERATOR second = begin;
        second++;
        return CardCombo(begin, second); // 那么就出第一张牌……
    }


    /**
     * 从指定手牌中寻找第一个能大过当前牌组的牌组
     * 如果随便出的话只出第一张
     * 如果不存在则返回一个PASS的牌组
     */
    template <typename CARD_ITERATOR>
    CardCombo findFirstValid_naive (CARD_ITERATOR begin, CARD_ITERATOR end) {
        // 然后先看一下是不是火箭，是的话就过
        if (comboType == CardComboType::ROCKET)
            return CardCombo();

        // 现在打算从手牌中凑出同牌型的牌
        auto deck = vector<Card>(begin, end); // 手牌
        short counts[MAX_LEVEL + 1] = {};

        unsigned short kindCount = 0;

        // 先数一下手牌里每种牌有多少个
        for (Card c : deck)
            counts[card2level(c)]++;

        // 手牌如果不够用，直接不用凑了，看看能不能炸吧
        if (deck.size() < cards.size())
            goto failure;

        // 再数一下手牌里有多少种牌
        for (short c : counts)
            if (c)
                kindCount++;

        // 否则不断增大当前牌组的主牌，看看能不能找到匹配的牌组
        {
            // 开始增大主牌
            int mainPackCount = findMaxSeq();
            bool isSequential =
                    comboType == CardComboType::STRAIGHT ||
                    comboType == CardComboType::STRAIGHT2 ||
                    comboType == CardComboType::PLANE ||
                    comboType == CardComboType::PLANE1 ||
                    comboType == CardComboType::PLANE2 ||
                    comboType == CardComboType::SSHUTTLE ||
                    comboType == CardComboType::SSHUTTLE2 ||
                    comboType == CardComboType::SSHUTTLE4;
            for (Level i = 1; ; i++) // 增大多少
            {
                for (int j = 0; j < mainPackCount; j++)
                {
                    int level = packs[j].level + i;

                    // 各种连续牌型的主牌不能到2，非连续牌型的主牌不能到小王，单张的主牌不能超过大王
                    if ((comboType == CardComboType::SINGLE && level > MAX_LEVEL) ||
                        (isSequential && level > MAX_STRAIGHT_LEVEL) ||
                        (comboType != CardComboType::SINGLE && !isSequential && level >= level_joker))
                        goto failure;

                    // 如果手牌中这种牌不够，就不用继续增了
                    if (counts[level] < packs[j].count)
                        goto next;
                }

                {
                    // 找到了合适的主牌，那么从牌呢？
                    // 如果手牌的种类数不够，那从牌的种类数就不够，也不行
                    if (kindCount < packs.size())
                        continue;

                    // 好终于可以了
                    // 计算每种牌的要求数目吧
                    short requiredCounts[MAX_LEVEL + 1] = {};
                    for (int j = 0; j < mainPackCount; j++)
                        requiredCounts[packs[j].level + i] = packs[j].count;
                    for (unsigned j = mainPackCount; j < packs.size(); j++)
                    {
                        Level k;
                        for (k = 0; k <= MAX_LEVEL; k++)
                        {
                            if (requiredCounts[k] || counts[k] < packs[j].count)
                                continue;
                            requiredCounts[k] = packs[j].count;
                            break;
                        }
                        if (k == MAX_LEVEL + 1) // 如果是都不符合要求……就不行了
                            goto next;
                    }


                    // 开始产生解
                    vector<Card> solve;
                    for (Card c : deck)
                    {
                        Level level = card2level(c);
                        if (requiredCounts[level])
                        {
                            solve.push_back(c);
                            requiredCounts[level]--;
                        }
                    }
                    return CardCombo(solve.begin(), solve.end());
                }

                next:
                ; // 再增大
            }
        }

        failure:
        // 实在找不到啊
        // 最后看一下能不能炸吧

        for (Level i = 0; i < level_joker; i++)
            if (counts[i] == 4 && (comboType != CardComboType::BOMB || i > packs[0].level)) // 如果对方是炸弹，能炸的过才行
            {
                // 还真可以啊……
                Card bomb[] = { Card(i * 4), Card(i * 4 + 1), Card(i * 4 + 2), Card(i * 4 + 3) };
                return CardCombo(bomb, bomb + 4);
            }

        // 有没有火箭？
        if (counts[level_joker] + counts[level_JOKER] == 2)
        {
            Card rocket[] = { card_joker, card_JOKER };
            return CardCombo(rocket, rocket + 2);
        }

        // ……
        return CardCombo();
    }



    template <typename CARD_ITERATOR>
    CardCombo findFirstValid(CARD_ITERATOR begin, CARD_ITERATOR end)
    {
        /*for (auto i = begin; i != end; ++i) {
            cout << *i << " ";
        }
        cout << endl;*/

        // 增加最简单的拆牌算法
        // by尹晨桥 5.15
        auto levelCount = levelCount_naive(begin, end);
        CardCombo nowCombo = CardCombo(begin, end);

        if (comboType == CardComboType::PASS) // 如果不需要大过谁，只需要随便出
            return findFirstValid_ifPass(begin, end);

        // 如果剩余的牌一起出能管，就一起出了
        // by尹晨桥 5.20
        if (canBeBeatenBy(nowCombo)) return nowCombo;

        // 增加农民乙不能管农民甲的判断
        // by尹晨桥 5.19
        if (myPosition == 2) {

            // 炸弹不管
            if (comboType == CardComboType::BOMB) return CardCombo();

            // 地主不要，农民甲要，农民乙不管
            // 除非农民乙的手牌数比农民甲小，或者农民甲出的牌比较小（不带人）
            if ((--whatTheyPlayed[0].end()) -> empty()
                && !((--whatTheyPlayed[1].end()) -> empty())
                && cardRemaining[1] > cardRemaining[2]
                && comboLevel >= 8) {
                return CardCombo();
            }

            // 农民甲出的牌已经比较大了，农民乙不管
            CardCombo tmp = CardCombo((--whatTheyPlayed[1].end()) -> begin(), (--whatTheyPlayed[1].end()) -> end());
            if (tmp.comboLevel >= 10 && cardRemaining[myPosition] >= 5)
                return CardCombo();
        }

        // 增加农民甲不管农民乙的判断
        // by尹晨桥 5.19
        if (myPosition == 1) {
            CardCombo tmp = CardCombo((--whatTheyPlayed[2].end()) -> begin(), (--whatTheyPlayed[2].end()) -> end());
            // 如果地主没出，农民乙出的牌已经比较大了，农民甲不管
            if ((--whatTheyPlayed[0].end()) -> empty()
                && tmp.comboLevel >= 10
                && cardRemaining[myPosition] >= 5)
                return CardCombo();
            // 如果农民甲只能拿大牌管农民乙的小牌，农民甲不管
            if (comboType == CardComboType::SINGLE
                && (--whatTheyPlayed[0].end()) -> empty()
                && tmp.comboLevel <= 7
                && (levelCount.single.empty() || levelCount.single[0] >= MAX_STRAIGHT_LEVEL)
                && cardRemaining[myPosition] >= 5
                && cardRemaining[0] != 1)
                return CardCombo();
            if (comboType == CardComboType::PAIR
                && (--whatTheyPlayed[0].end()) -> empty()
                && tmp.comboLevel <= 7
                && (levelCount.pair.empty() || levelCount.pair[0] >= MAX_STRAIGHT_LEVEL)
                && cardRemaining[myPosition] >= 5
                && cardRemaining[0] != 2)
                return CardCombo();
        }

        // 增加跟牌时不拆牌的判断
        // by尹晨桥 5.19
        // 跟单牌
        if (comboType == CardComboType::SINGLE) {
            // 先搜索单张
            for (auto i = levelCount.single.begin(); i != levelCount.single.end(); ++i) {
                // 如果既有2又有王，优先出2
                if ((*i == level_joker || *i == level_JOKER) && 12 > packs[0].level) {
                    if (levelCount.countEachLevel[12] != 0)
                        return makeCardComboFromLevel(12, 1);
                }
                if (*i > packs[0].level) {
                    return makeCardComboFromLevel(*i, 1);
                }
            }
            // 单张搜索不到，再搜索对子
            for (auto i = --levelCount.pair.end(); i != --levelCount.pair.begin(); --i) {
                if (*i > packs[0].level) {
                    return makeCardComboFromLevel(*i, 1);
                }
            }
            // 对子搜索不到，再搜索三条
            for (auto i = --levelCount.triplet.end(); i != --levelCount.triplet.begin(); --i) {
                if (*i > packs[0].level && (*i >= 11 || cardRemaining[myPosition] <= 10)) {
                    return makeCardComboFromLevel(*i, 1);
                }
            }
            // 非残局情况下不拆牌
            if (cardRemaining[0] >= 10 && cardRemaining[1] >= 10 && cardRemaining[2] >= 10)
                return CardCombo();
        }

        // 跟对子
        if (comboType == CardComboType::PAIR) {
            // 先搜索对子
            for (auto i = levelCount.pair.begin(); i != levelCount.pair.end(); ++i) {
                if (*i > packs[0].level) {
                    return makeCardComboFromLevel(*i, 2);
                }
            }
            // 残局情况下可以拆三条
            if ((myPosition == 0 && cardRemaining[1] <= 6 && cardRemaining[2] <= 6) ||
                    (myPosition == 1 && cardRemaining[0] <= 6) ||
                    (myPosition == 2 && cardRemaining[0] <= 6)) {
                for (auto i = --levelCount.triplet.end(); i != --levelCount.triplet.begin(); --i) {
                    if (*i > packs[0].level) {
                        return makeCardComboFromLevel(*i, 2);
                    }
                }
            }
            // 非残局不拆牌
            if (cardRemaining[0] >= 10 && cardRemaining[1] >= 10 && cardRemaining[2] >= 10)
                return CardCombo();
        }

        // 跟三带一、三带二
        // by尹晨桥 5.20
        // 跟三带一
        if (comboType == CardComboType::TRIPLET1) {

            // 先搜索一下主牌
            CardCombo Triplet;
            for (auto i = levelCount.triplet.begin(); i != levelCount.triplet.end(); ++i) {
                if (*i > packs[0].level) {
                    Triplet = makeCardComboFromLevel(*i, 3);
                    break;
                }
            }

            // 如果找不到，使用无脑策略（无脑策略考虑能不能炸和火箭）
            if (Triplet.cards.empty()) {
                auto naiveValid = findFirstValid_naive(begin, end);
                // 如果找不到，农民乙不炸农民甲
                if (myPosition == 2 && !((--whatTheyPlayed[1].end()) -> empty()))
                    return CardCombo();
                return naiveValid;
            }

            // 如果找得到，复制一下
            vector<Card> tmp(Triplet.cards.begin(), Triplet.cards.end());

            // 再搜索从牌
            // 如果有单牌，先带单牌
            if (!levelCount.single.empty()) {
                auto Single = makeCardComboFromLevel(levelCount.single[0], 1);
                tmp.push_back(Single.cards[0]);
                return CardCombo(tmp.begin(), tmp.end());
            }
            // 如果没有单牌有对子，且对子不带人，则拆对子
            if (!levelCount.pair.empty()) {
                if (levelCount.pair[0] <= 7) {
                    auto Single = makeCardComboFromLevel(levelCount.pair[0], 1);
                    tmp.push_back(Single.cards[0]);
                    return CardCombo(tmp.begin(), tmp.end());
                }
            }
            // 看看有没有炸弹
            // 农民乙不炸农民甲
            if (!levelCount.bomb.empty() &&
                !(myPosition == 2 && !((--whatTheyPlayed[1].end()) -> empty())))
                return makeCardComboFromLevel(levelCount.bomb[0], 4);

            // 如果都没有，则按照无脑算法出
        }

        // 跟三带二
        if (comboType == CardComboType::TRIPLET2) {

            // 先搜索一下主牌
            CardCombo Triplet;
            for (auto i = levelCount.triplet.begin(); i != levelCount.triplet.end(); ++i) {
                if (*i > packs[0].level) {
                    Triplet = makeCardComboFromLevel(*i, 3);
                    break;
                }
            }

            // 如果找不到，使用无脑策略（无脑策略考虑能不能炸和火箭）
            if (Triplet.cards.empty()) {
                auto naiveValid = findFirstValid_naive(begin, end);
                // 如果找不到，农民乙不炸农民甲
                if (myPosition == 2 && !((--whatTheyPlayed[1].end()) -> empty()))
                    return CardCombo();
                return naiveValid;
            }

            // 如果找得到，复制一下
            vector<Card> tmp(Triplet.cards.begin(), Triplet.cards.end());

            // 再搜索从牌
            // 如果有对子，先带对子
            if (!levelCount.pair.empty()) {
                auto Pair = makeCardComboFromLevel(levelCount.pair[0], 2);
                tmp.push_back(Pair.cards[0]);
                tmp.push_back(Pair.cards[1]);
                return CardCombo(tmp.begin(), tmp.end());
            }
            // 如果没有对子有三条，且三条不带人，则拆三条
            if (!levelCount.triplet.empty()) {
                if (levelCount.triplet[0] <= 7) {
                    auto Pair = makeCardComboFromLevel(levelCount.triplet[0], 2);
                    tmp.push_back(Pair.cards[0]);
                    tmp.push_back(Pair.cards[1]);
                    return CardCombo(tmp.begin(), tmp.end());
                }
            }
            // 看看有没有炸弹
            // 农民乙不炸农民甲
            if (!levelCount.bomb.empty() &&
                    !(myPosition == 2 && !((--whatTheyPlayed[1].end()) -> empty())))
                return makeCardComboFromLevel(levelCount.bomb[0], 4);

            // 如果再搜索不到，就要拆炸弹了，不行，直接过
            return CardCombo();
        }

        // 如果自己写的算法找不到，就用无脑算法
        auto naiveValid = findFirstValid_naive(begin, end);

        // 农民乙不炸农民甲的牌
        if (myPosition == 2
            && !((--whatTheyPlayed[1].end()) -> empty())
            && (naiveValid.comboType == CardComboType::BOMB || naiveValid.comboType == CardComboType::ROCKET)) {
            return CardCombo();
        }
        return naiveValid;
    }

    void debugPrint()
    {
#ifndef _BOTZONE_ONLINE
        cout << "【" << cardComboStrings[(int)comboType] <<
             "共" << cards.size() << "张，大小序" << comboLevel << "】" << endl;
#endif
    }
};

// 当前要出的牌需要大过谁
CardCombo lastValidCombo;

namespace BotzoneIO
{
    using namespace std;
    void input()
    {
        // 读入输入（平台上的输入是单行）
        string line;
        getline(cin, line);
        Json::Value input;
        Json::Reader reader;
        reader.parse(line, input);

        // 首先处理第一回合，得知自己是谁、有哪些牌
        {
            auto firstRequest = input["requests"][0u]; // 下标需要是 unsigned，可以通过在数字后面加u来做到
            auto own = firstRequest["own"];
            auto llpublic = firstRequest["public"];
            auto history = firstRequest["history"];
            for (unsigned i = 0; i < own.size(); i++)
                myCards.insert(own[i].asInt());
            for (unsigned i = 0; i < llpublic.size(); i++)
                landlordPublicCards.insert(llpublic[i].asInt());
            if (history[0u].size() == 0)
                if (history[1].size() == 0)
                    myPosition = 0; // 上上家和上家都没出牌，说明是地主
                else
                    myPosition = 1; // 上上家没出牌，但是上家出牌了，说明是农民甲
            else
                myPosition = 2; // 上上家出牌了，说明是农民乙
        }

        // history里第一项（上上家）和第二项（上家）分别是谁的决策
        int whoInHistory[] = { (myPosition - 2 + PLAYER_COUNT) % PLAYER_COUNT, (myPosition - 1 + PLAYER_COUNT) % PLAYER_COUNT };

        int turn = input["requests"].size();
        for (int i = 0; i < turn; i++)
        {
            // 逐次恢复局面到当前
            auto history = input["requests"][i]["history"]; // 每个历史中有上家和上上家出的牌
            int howManyPass = 0;
            for (int p = 0; p < 2; p++)
            {
                int player = whoInHistory[p]; // 是谁出的牌
                auto playerAction = history[p]; // 出的哪些牌
                vector<Card> playedCards;
                for (unsigned _ = 0; _ < playerAction.size(); _++) // 循环枚举这个人出的所有牌
                {
                    int card = playerAction[_].asInt(); // 这里是出的一张牌
                    playedCards.push_back(card);
                }
                whatTheyPlayed[player].push_back(playedCards); // 记录这段历史
                cardRemaining[player] -= playerAction.size();

                if (playerAction.size() == 0)
                    howManyPass++;
                else
                    lastValidCombo = CardCombo(playedCards.begin(), playedCards.end());
            }

            if (howManyPass == 2)
                lastValidCombo = CardCombo();

            if (i < turn - 1)
            {
                // 还要恢复自己曾经出过的牌
                auto playerAction = input["responses"][i]; // 出的哪些牌
                vector<Card> playedCards;
                for (unsigned _ = 0; _ < playerAction.size(); _++) // 循环枚举自己出的所有牌
                {
                    int card = playerAction[_].asInt(); // 这里是自己出的一张牌
                    myCards.erase(card); // 从自己手牌中删掉
                    playedCards.push_back(card);
                }
                whatTheyPlayed[myPosition].push_back(playedCards); // 记录这段历史
                cardRemaining[myPosition] -= playerAction.size();
            }
        }
    }

    /**
     * 输出决策，begin是迭代器起点，end是迭代器终点
     * CARD_ITERATOR是Card（即short）类型的迭代器
     */
    template <typename CARD_ITERATOR>
    void output(CARD_ITERATOR begin, CARD_ITERATOR end)
    {
        Json::Value result, response(Json::arrayValue);
        for (; begin != end; begin++)
            response.append(*begin);
        result["response"] = response;

        Json::FastWriter writer;
        cout << writer.write(result) << endl;
    }
}

int main()
{
    BotzoneIO::input();

    // 做出决策（你只需修改以下部分）

    // findFirstValid 函数可以用作修改的起点
    CardCombo myAction = lastValidCombo.findFirstValid(myCards.begin(), myCards.end());
    //myAction.debugPrint();
    //cout << "myPosition: " << myPosition << endl;
    /*for (auto i = myCards.begin(); i != myCards.end(); ++i) {
        cout << card2level(*i) << " ";
    }
    cout << endl;*/

    // 是合法牌
    /*assert(myAction.comboType != CardComboType::INVALID);

    assert(
    // 在上家没过牌的时候过牌
            (lastValidCombo.comboType != CardComboType::PASS && myAction.comboType == CardComboType::PASS) ||
            // 在上家没过牌的时候出打得过的牌
            (lastValidCombo.comboType != CardComboType::PASS && lastValidCombo.canBeBeatenBy(myAction)) ||
            // 在上家过牌的时候出合法牌
            (lastValidCombo.comboType == CardComboType::PASS && myAction.comboType != CardComboType::INVALID)
    );*/

    // 决策结束，输出结果（你只需修改以上部分）

    BotzoneIO::output(myAction.cards.begin(), myAction.cards.end());
}