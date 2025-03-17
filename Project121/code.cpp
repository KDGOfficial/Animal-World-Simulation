#include <iostream>
#include <vector>
#include <memory>

using namespace std;

class Herbivore {
protected:
    int weight;
    bool life;
public:
    Herbivore(int w) : weight(w), life(true) {}
    virtual void EatGrass() { weight += 10; }
    int GetWeight() const { return weight; }
    bool IsAlive() const { return life; }
    void Die() { life = false; }
    virtual ~Herbivore() = default;
};

class Carnivore {
protected:
    int power;
public:
    Carnivore(int p) : power(p) {}
    virtual void Eat(Herbivore& herbivore) {
        if (power > herbivore.GetWeight()) {
            power += 10;
            herbivore.Die();
        }
        else {
            power -= 10;
        }
    }
    int GetPower() const { return power; }
    virtual ~Carnivore() = default;
};

class Wildebeest : public Herbivore {
public:
    Wildebeest() : Herbivore(50) {}
};

class Bison : public Herbivore {
public:
    Bison() : Herbivore(60) {}
};

class Elk : public Herbivore {
public:
    Elk() : Herbivore(70) {}
};

class Lion : public Carnivore {
public:
    Lion() : Carnivore(80) {}
};

class Wolf : public Carnivore {
public:
    Wolf() : Carnivore(70) {}
};

class Tiger : public Carnivore {
public:
    Tiger() : Carnivore(90) {}
};

class Continent {
public:
    virtual vector<shared_ptr<Herbivore>> CreateHerbivores() = 0;
    virtual vector<shared_ptr<Carnivore>> CreateCarnivores() = 0;
    virtual ~Continent() = default;
};

class Africa : public Continent {
public:
    vector<shared_ptr<Herbivore>> CreateHerbivores() override {
        return { make_shared<Wildebeest>() };
    }
    vector<shared_ptr<Carnivore>> CreateCarnivores() override {
        return { make_shared<Lion>() };
    }
};

class NorthAmerica : public Continent {
public:
    vector<shared_ptr<Herbivore>> CreateHerbivores() override {
        return { make_shared<Bison>() };
    }
    vector<shared_ptr<Carnivore>> CreateCarnivores() override {
        return { make_shared<Wolf>() };
    }
};

class Eurasia : public Continent {
public:
    vector<shared_ptr<Herbivore>> CreateHerbivores() override {
        return { make_shared<Elk>() };
    }
    vector<shared_ptr<Carnivore>> CreateCarnivores() override {
        return { make_shared<Tiger>() };
    }
};

class AnimalWorld {
private:
    vector<shared_ptr<Herbivore>> herbivores;
    vector<shared_ptr<Carnivore>> carnivores;
public:
    AnimalWorld(shared_ptr<Continent> continent) {
        herbivores = continent->CreateHerbivores();
        carnivores = continent->CreateCarnivores();
    }
    void MealsHerbivores() {
        for (auto& h : herbivores) {
            h->EatGrass();
        }
    }
    void NutritionCarnivores() {
        for (auto& c : carnivores) {
            for (auto& h : herbivores) {
                if (h->IsAlive()) {
                    c->Eat(*h);
                    break;
                }
            }
        }
    }
    void ShowStats() {
        cout << "Herbivores:\n";
        for (auto& h : herbivores) {
            cout << "Weight: " << h->GetWeight() << ", Alive: " << (h->IsAlive() ? "Yes" : "No") << endl;
        }
        cout << "Carnivores:\n";
        for (auto& c : carnivores) {
            cout << "Power: " << c->GetPower() << endl;
        }
    }
};

int main() {
    shared_ptr<Continent> africa = make_shared<Africa>();
    shared_ptr<Continent> northAmerica = make_shared<NorthAmerica>();
    shared_ptr<Continent> eurasia = make_shared<Eurasia>();

    AnimalWorld worldAfrica(africa);
    AnimalWorld worldNA(northAmerica);
    AnimalWorld worldEurasia(eurasia);

    worldAfrica.MealsHerbivores();
    worldAfrica.NutritionCarnivores();
    worldAfrica.ShowStats();

    worldNA.MealsHerbivores();
    worldNA.NutritionCarnivores();
    worldNA.ShowStats();

    worldEurasia.MealsHerbivores();
    worldEurasia.NutritionCarnivores();
    worldEurasia.ShowStats();

    return 0;
}
