#include <random>

class AbstractDice{
    public:
        virtual unsigned roll() = 0;
        AbstractDice(){}
};

class Dice : public AbstractDice{
    public:
        Dice(unsigned max, unsigned seed):
            max(max), dstr(1, max), reng(seed) { }
        
        virtual unsigned roll() override {
            return dstr(reng);
        }
        Dice(): max(1568), dstr(1, max), reng(0){}

    private:
        unsigned max;
        std::uniform_int_distribution<unsigned> dstr;
        std::default_random_engine reng;
        
};

class ThreeDicePool : public AbstractDice{
    public:
        ThreeDicePool(unsigned seed_1, unsigned seed_2, unsigned seed_3)
        {
            dices[0] = new Dice(1568,seed_1);
            dices[1] = new Dice(1568,seed_2);
            dices[2] = new Dice(1568,seed_3);
        }
        
        unsigned roll() override{
            return dices[0]->roll() + dices[1]->roll() + dices[2]->roll();
        }
    private: 
        AbstractDice* dices[3];
};

class PenaltyDice : public virtual AbstractDice{
    public:
        PenaltyDice(unsigned seed){
            dice = new Dice(1568, seed);
        }
        PenaltyDice(Dice& dice)
        {this->dice = &dice;}
        unsigned roll() override{
            return std::min<unsigned>(dice->roll(), dice->roll());
        }
    private:
        AbstractDice* dice;
};

class BonusDice : public virtual AbstractDice{
    public:
        BonusDice(unsigned seed){
            dice = new Dice(1568, seed);
        }
        BonusDice(Dice& dice)
        {this->dice = &dice;}
        unsigned roll() override{
            return std::min<unsigned>(dice->roll(), dice->roll());
        }
    private:
        AbstractDice* dice;
};

double expected_value(AbstractDice &d, unsigned number_of_rolls = 1) {
    auto accum = 0llu;
    for (unsigned cnt = 0; cnt != number_of_rolls; ++cnt)
    accum += d.roll();
    return
    static_cast<double>(accum) / static_cast<double>(number_of_rolls);
};

class DoubleDice : public virtual AbstractDice, private PenaltyDice, private BonusDice
{
    public:
        DoubleDice(Dice& dice): PenaltyDice(dice), BonusDice(dice) {}

        unsigned roll() override{
            return PenaltyDice::roll() + static_cast<BonusDice*>(this)->roll(); }


};