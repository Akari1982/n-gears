#ifndef ONTOGENESIS_H
#define ONTOGENESIS_H

#include <vector>
#include <OgreString.h>

#include "Cell.h"
#include "Entity.h"
#include "Logger.h"



class Ontogenesis
{
public:
    Ontogenesis( const Ogre::String& file_prefix, const int type );
    virtual ~Ontogenesis();

    void Draw( const unsigned int x, const unsigned int y );

    void LoadNetwork( Entity* entity );
    void EntityDeath( Entity* entity );

private:
    Ontogenesis();

public:
    enum ConditionType
    {
        // сигналом для дальнейшей экспрессии в клетках может быть
        C_O_PROTEIN = 0,
        C_NO_PROTEIN,
        C_I_PROTEIN,
        C_NI_PROTEIN,

        C_TOTAL
    };

    enum ExpressionType
    {
        // реальные клетки могут делится на 2 одинаковые и на 2 неодинаковые части.
        // протеины заключенные в клетке могут поделится полностью или
        // каждый отдельный протеин может сконцентрироватся только в одной дочерней клетке
        // клетка делится только на 2 части
        // [bool = 1] - внутренний протеин наследуется
        E_SPLIT = 0,
        E_SPAWN,
        E_MIGRATE,
        E_O_PROTEIN,
        E_I_PROTEIN,
        E_DENDRITE,
        E_DENDRITE_I,
        E_AXON,
        E_AXON_I,

        E_TOTAL
   };

    struct Condition
    {
        ConditionType type;
        int value_i[ 1 ];
        int value_f[ 1 ];
    };

    struct Expression
    {
        ExpressionType type;
        int value_i[ 3 ];
    };

    struct Gene
    {
        unsigned int unique_id;
        float conserv;
        std::vector< Condition > cond;
        std::vector< Expression > expr;
    };

    struct Species
    {
        std::vector< Gene > genome;
        std::vector< Cell* > network;
    };

    struct Generation
    {
        float top_fitness;
        size_t top_id;
        std::vector< Gene > base_genome;
        std::vector< Species > species;

        Logger* file;
    };

private:
    struct PowerProtein
    {
        float power;
        size_t cell_id;
    };
    float SearchOuterProtein( std::vector< Cell* >& network, const int protein, const int x, const int y, std::vector< PowerProtein >& powers );
    bool FindPlaceForCell( std::vector< Cell* >& network, const int x, const int y, const int radius, int &new_x, int &new_y );
    bool IsCell( std::vector< Cell* >& network, const int x, const int y );
    std::vector< Gene > Mutate( std::vector< Gene >& genome );
    Condition GenerateRandomCondition();
    void GenerateRandomConditionValue( Condition& cond );
    Expression GenerateRandomExpression();
    void GenerateRandomExpressionValue( Expression& expr );

    Ogre::String ConditionTypeToString( const ConditionType type );
    Ogre::String ExpressionTypeToString( const ExpressionType type );
    void DumpGenome( Logger* file, std::vector< Gene >& genome );

private:
    int m_Type;
    Ogre::String m_FilePrefix;

    unsigned int m_GeneUniqueId;
    std::vector< Generation > m_Generations;
};



#endif // ONTOGENESIS_H
