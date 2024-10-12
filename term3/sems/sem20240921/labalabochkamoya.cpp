#include <random>
#include <iostream>
#include <fstream>

class AbstractDice{
    public:
        virtual unsigned roll() = 0;
        AbstractDice(){}
        virtual ~AbstractDice(){}
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
        ThreeDicePool(unsigned max, unsigned seed_1, unsigned seed_2, unsigned seed_3)
        {
            dices[0] = new Dice(max,seed_1);
            dices[1] = new Dice(max,seed_2);
            dices[2] = new Dice(max,seed_3);
        }
        ThreeDicePool(AbstractDice& dice1, AbstractDice& dice2, AbstractDice& dice3)
        {
            dices[0] = &dice1;
            dices[1] = &dice2;
            dices[2] = &dice3;
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
            return std::max<unsigned>(dice->roll(), dice->roll());
        }
    private:
        AbstractDice* dice;
};



class DoubleDice : public virtual AbstractDice, private PenaltyDice, private BonusDice
{
    public:
        DoubleDice(Dice& dice): PenaltyDice(dice), BonusDice(dice) {}

        unsigned roll() override{
            return PenaltyDice::roll() + BonusDice::roll(); }

};

class DoubleDice_part_two : public virtual AbstractDice
{
    public:
        DoubleDice_part_two(Dice& dice){
            ploh = new PenaltyDice(dice);
            horosh = new BonusDice(dice);
        }
        unsigned roll() override{
            return ploh->roll() + horosh->roll(); }

    private:
        PenaltyDice* ploh;
        BonusDice* horosh;
};

double expected_value(AbstractDice &d, unsigned number_of_rolls = 1) {
    auto accum = 0llu;
    for (unsigned cnt = 0; cnt != number_of_rolls; ++cnt)
    {
        accum += d.roll();
    }

    return static_cast<double>(accum) / static_cast<double>(number_of_rolls);
};

double value_probability(unsigned value, AbstractDice &d, unsigned number_of_rolls = 100){
    auto accum = 0llu;
    for (unsigned cnt = 0; cnt != number_of_rolls; ++cnt)
    {
        accum += d.roll() == value;
    }

    return static_cast<double>(accum) / static_cast<double>(number_of_rolls);
};

double* value_probability_adecvatnoe(AbstractDice &d, unsigned number_of_rolls = 1000, unsigned max = 100){
    double* vosvrat_ozona = new double[max];
    for (unsigned cnt = 0; cnt != number_of_rolls; ++cnt)
    {
        vosvrat_ozona[d.roll()-1]++;
    }
    for (unsigned cnt = 0; cnt != max; ++cnt)
    {
        vosvrat_ozona[cnt]/=number_of_rolls;
    }

    return vosvrat_ozona;
}

int main()
{
    auto Stas = Dice(6, 2345676543);
    auto Roma = ThreeDicePool(6, 5678, 674839, 346789);
    std::cout << expected_value(Stas, 4569) << " Стаса покидали, теперь кидаем Рому-триединого: " << expected_value(Roma, 513) << "\n";
    auto sotka = Dice(100, 456787654);
    auto uuuuuud = PenaltyDice(sotka);
    auto pacanskii_otl = BonusDice(sotka);

    int number_of_broski = 100000;

    double* plohoi_cubik = value_probability_adecvatnoe(uuuuuud, number_of_broski);
    double* horoshii_cubik = value_probability_adecvatnoe(pacanskii_otl, number_of_broski);
    double* a_ti_tut_otkuda = value_probability_adecvatnoe(Roma, number_of_broski);
    std::ofstream output_file;
    output_file.open("vivod.csv");
    output_file << "грань,так себе кубик,зашибись кубик,РОМА\n";

    for(int i = 0; i!=100; i++){
        output_file << i+1 << "," << plohoi_cubik[i] << "," << horoshii_cubik[i]<< "," << a_ti_tut_otkuda[i]<<"\n";
    }
    auto kiwi = DoubleDice(sotka); //положил сотку на киви окда
    auto thomas_shelbi = DoubleDice_part_two(sotka); //ну меня убил бы если бы я ему сотку не отдал

    double* cubik_no_wings = value_probability_adecvatnoe(kiwi, number_of_broski, 200);
    
    double* cubik_shelbi = value_probability_adecvatnoe(thomas_shelbi, number_of_broski, 200);
    std::ofstream output_file_2;
    output_file_2.open("vivod_2.csv");
    output_file_2 << "грань,птичка,томас шелби\n";
    for(int i = 0; i!=200; i++){
        output_file_2 << i+1 << "," << cubik_no_wings[i] << "," << cubik_shelbi[i] << "\n";
    }



}