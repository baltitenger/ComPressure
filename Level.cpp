#include "Level.h"

Level::Level(unsigned level_index_):
    level_index(level_index_),
    sim_points(128)
{
    circuit = new Circuit;
    init();
}

Level::Level(unsigned level_index_, SaveObject* sobj):
    level_index(level_index_),
    sim_points(128)
{
    if (sobj)
        circuit = new Circuit(sobj->get_map()->get_item("circuit")->get_map());
    else
        circuit = new Circuit;
    init();
}

SaveObject* Level::save()
{
    SaveObjectMap* omap = new SaveObjectMap;
    omap->add_item("circuit", circuit->save());
    return omap;
}

void Level::add_sim_point(SimPoint point, unsigned count)
{
    for (unsigned i = 0; i < count; i++)
    {
        sim_points[sim_point_count] = point;

        sim_point_count++;
        assert(sim_point_count <  128);
    }
}

SDL_Rect Level::getimage(Direction direction)
{
    int mask = connection_mask << direction | connection_mask >> (4 - direction) & 0xF;
    return SDL_Rect{128 + (mask & 0x3) * 32, 32 + ((mask >> 2) & 0x3) * 32, 32, 32};
}

SDL_Rect Level::getimage_fg(Direction direction)
{
    return SDL_Rect{direction * 24, 182 + (int(level_index) * 24), 24, 24};
}


void Level::init(void)
{
#define DRIVE(a)   IOValue(a, 100, a)
#define FLOAT(A)   IOValue(0, 0, A)

//                          N E S W
#define CONMASK_ALL        (1|2|4|8)
#define CONMASK_WE         (  2|  8)
#define CONMASK_WES        (  2|4|8)
#define CONMASK_NES        (1|2|4  )

    switch(level_index)
    {
        case 0:              //       N            E             S            W                  time
            add_sim_point(SimPoint(   DRIVE(0)    ,DRIVE(0)     ,DRIVE(0)    ,DRIVE(0)    ),     1);
            add_sim_point(SimPoint(   DRIVE(100)  ,FLOAT(0)     ,FLOAT(100)  ,FLOAT(0)    ),     1);
            add_sim_point(SimPoint(   DRIVE(0)    ,DRIVE(0)     ,DRIVE(0)    ,DRIVE(0)    ),     1);
            add_sim_point(SimPoint(   FLOAT(100)  ,FLOAT(0)     ,DRIVE(100)  ,FLOAT(0)    ),     1);
            add_sim_point(SimPoint(   DRIVE(0)    ,DRIVE(0)     ,DRIVE(0)    ,DRIVE(0)    ),     1);
            add_sim_point(SimPoint(   FLOAT(0)    ,DRIVE(100)   ,FLOAT(0)    ,FLOAT(100)  ),     1);
            add_sim_point(SimPoint(   DRIVE(0)    ,DRIVE(0)     ,DRIVE(0)    ,DRIVE(0)    ),     1);
            add_sim_point(SimPoint(   FLOAT(0)    ,FLOAT(100)   ,FLOAT(0)    ,DRIVE(100)  ),     1);
            add_sim_point(SimPoint(   DRIVE(0)    ,DRIVE(0)     ,DRIVE(0)    ,DRIVE(0)    ),     1);

            add_sim_point(SimPoint(   DRIVE(100)  ,DRIVE(100)   ,DRIVE(100)  ,DRIVE(100)  ),     1);
            add_sim_point(SimPoint(   DRIVE(50)   ,FLOAT(100)   ,FLOAT(50)   ,FLOAT(100)  ),     1);
            add_sim_point(SimPoint(   DRIVE(100)  ,DRIVE(100)   ,DRIVE(100)  ,DRIVE(100)  ),     1);
            add_sim_point(SimPoint(   FLOAT(50)   ,FLOAT(100)   ,DRIVE(50)   ,FLOAT(100)  ),     1);
            add_sim_point(SimPoint(   DRIVE(100)  ,DRIVE(100)   ,DRIVE(100)  ,DRIVE(100)  ),     1);
            add_sim_point(SimPoint(   FLOAT(100)  ,DRIVE(50)    ,FLOAT(100)  ,FLOAT(50)   ),     1);
            add_sim_point(SimPoint(   DRIVE(100)  ,DRIVE(100)   ,DRIVE(100)  ,DRIVE(100)  ),     1);
            add_sim_point(SimPoint(   FLOAT(100)  ,FLOAT(50)    ,FLOAT(100)  ,DRIVE(50)   ),     1);
            add_sim_point(SimPoint(   DRIVE(100)  ,DRIVE(100)   ,DRIVE(100)  ,DRIVE(100)  ),     1);
            
            substep_count = 2000;
            connection_mask = CONMASK_ALL;
            break;

        case 1:              //       N            E             S            W                  time
            add_sim_point(SimPoint(   DRIVE(0)    ,DRIVE(0)     ,DRIVE(100)  ,DRIVE(0)     ),     1);
            add_sim_point(SimPoint(   DRIVE(0)    ,FLOAT(0)     ,DRIVE(100)  ,DRIVE(100)   ),     1);
            add_sim_point(SimPoint(   DRIVE(0)    ,DRIVE(0)     ,DRIVE(100)  ,FLOAT(100)   ),     1);
            add_sim_point(SimPoint(   DRIVE(0)    ,DRIVE(100)   ,DRIVE(100)  ,DRIVE(0)     ),     1);
            add_sim_point(SimPoint(   DRIVE(0)    ,DRIVE(100)   ,DRIVE(100)  ,FLOAT(0)     ),     1);
            add_sim_point(SimPoint(   DRIVE(0)    ,DRIVE(100)   ,DRIVE(0)    ,FLOAT(100)   ),     1);
            add_sim_point(SimPoint(   DRIVE(0)    ,DRIVE(0)     ,DRIVE(0)    ,FLOAT(0)     ),     1);
            add_sim_point(SimPoint(   DRIVE(0)    ,FLOAT(100)   ,DRIVE(0)    ,DRIVE(100)   ),     1);
            add_sim_point(SimPoint(   DRIVE(0)    ,FLOAT(0)     ,DRIVE(0)    ,DRIVE(0)     ),     1);
                                                 
            substep_count = 3000;
            connection_mask = CONMASK_WES;
            break;


        case 2:              //       N            E             S            W                  time
            add_sim_point(SimPoint(   DRIVE(0)    ,DRIVE(0)     ,DRIVE(0)    ,DRIVE(0)     ),     1);
            add_sim_point(SimPoint(   DRIVE(0)    ,FLOAT(0)     ,DRIVE(0)    ,DRIVE(0)     ),     1);
            add_sim_point(SimPoint(   DRIVE(0)    ,FLOAT(10)    ,DRIVE(0)    ,DRIVE(10)    ),     1);
            add_sim_point(SimPoint(   DRIVE(0)    ,FLOAT(20)    ,DRIVE(0)    ,DRIVE(20)    ),     1);
            add_sim_point(SimPoint(   DRIVE(0)    ,FLOAT(30)    ,DRIVE(0)    ,DRIVE(30)    ),     1);
            add_sim_point(SimPoint(   DRIVE(0)    ,FLOAT(40)    ,DRIVE(0)    ,DRIVE(40)    ),     1);
            add_sim_point(SimPoint(   DRIVE(0)    ,FLOAT(50)    ,DRIVE(0)    ,DRIVE(50)    ),     1);
            add_sim_point(SimPoint(   DRIVE(0)    ,FLOAT(60)    ,DRIVE(0)    ,DRIVE(60)    ),     1);
            add_sim_point(SimPoint(   DRIVE(0)    ,FLOAT(70)    ,DRIVE(0)    ,DRIVE(70)    ),     1);
            add_sim_point(SimPoint(   DRIVE(0)    ,FLOAT(80)    ,DRIVE(0)    ,DRIVE(80)    ),     1);
            add_sim_point(SimPoint(   DRIVE(0)    ,FLOAT(90)    ,DRIVE(0)    ,DRIVE(90)    ),     1);
            add_sim_point(SimPoint(   DRIVE(0)    ,FLOAT(100)   ,DRIVE(0)    ,DRIVE(100)   ),     1);
            add_sim_point(SimPoint(   DRIVE(0)    ,FLOAT(80)    ,DRIVE(0)    ,DRIVE(80)    ),     1);
            add_sim_point(SimPoint(   DRIVE(0)    ,FLOAT(50)    ,DRIVE(0)    ,DRIVE(50)    ),     1);
            add_sim_point(SimPoint(   DRIVE(0)    ,FLOAT(90)    ,DRIVE(0)    ,DRIVE(90)    ),     1);
            add_sim_point(SimPoint(   DRIVE(0)    ,FLOAT(40)    ,DRIVE(0)    ,DRIVE(40)    ),     1);

            substep_count = 5000;
            connection_mask = CONMASK_WE;
            break;

        case 3:              //       N            E             S            W                  time
            add_sim_point(SimPoint(   DRIVE(0)    ,DRIVE(100)   ,DRIVE(0)    ,DRIVE(0)     ),     1);
            add_sim_point(SimPoint(   DRIVE(0)    ,FLOAT(100)   ,DRIVE(0)    ,DRIVE(0)     ),     1);
            add_sim_point(SimPoint(   DRIVE(0)    ,FLOAT(90)    ,DRIVE(0)    ,DRIVE(10)    ),     1);
            add_sim_point(SimPoint(   DRIVE(0)    ,FLOAT(80)    ,DRIVE(0)    ,DRIVE(20)    ),     1);
            add_sim_point(SimPoint(   DRIVE(0)    ,FLOAT(70)    ,DRIVE(0)    ,DRIVE(30)    ),     1);
            add_sim_point(SimPoint(   DRIVE(0)    ,FLOAT(60)    ,DRIVE(0)    ,DRIVE(40)    ),     1);
            add_sim_point(SimPoint(   DRIVE(0)    ,FLOAT(50)    ,DRIVE(0)    ,DRIVE(50)    ),     1);
            add_sim_point(SimPoint(   DRIVE(0)    ,FLOAT(40)    ,DRIVE(0)    ,DRIVE(60)    ),     1);
            add_sim_point(SimPoint(   DRIVE(0)    ,FLOAT(30)    ,DRIVE(0)    ,DRIVE(70)    ),     1);
            add_sim_point(SimPoint(   DRIVE(0)    ,FLOAT(20)    ,DRIVE(0)    ,DRIVE(80)    ),     1);
            add_sim_point(SimPoint(   DRIVE(0)    ,FLOAT(10)    ,DRIVE(0)    ,DRIVE(90)    ),     1);
            add_sim_point(SimPoint(   DRIVE(0)    ,FLOAT(0)     ,DRIVE(0)    ,DRIVE(100)   ),     1);
            add_sim_point(SimPoint(   DRIVE(0)    ,FLOAT(20)    ,DRIVE(0)    ,DRIVE(80)    ),     1);
            add_sim_point(SimPoint(   DRIVE(0)    ,FLOAT(50)    ,DRIVE(0)    ,DRIVE(50)    ),     1);
            add_sim_point(SimPoint(   DRIVE(0)    ,FLOAT(10)    ,DRIVE(0)    ,DRIVE(90)    ),     1);
            add_sim_point(SimPoint(   DRIVE(0)    ,FLOAT(60)    ,DRIVE(0)    ,DRIVE(40)    ),     1);

            substep_count = 5000;
            connection_mask = CONMASK_WE;
            break;

        case 4:              //       N            E             S            W                  time
            add_sim_point(SimPoint(   DRIVE(0)    ,FLOAT(0)     ,DRIVE(100)  ,DRIVE(0)     ),     1);
            add_sim_point(SimPoint(   DRIVE(0)    ,FLOAT(0)     ,DRIVE(0)    ,DRIVE(0)     ),     1);
            add_sim_point(SimPoint(   DRIVE(100)  ,FLOAT(100)   ,DRIVE(0)    ,DRIVE(0)     ),     1);
            add_sim_point(SimPoint(   DRIVE(100)  ,FLOAT(100)   ,DRIVE(100)  ,DRIVE(0)     ),     1);
            add_sim_point(SimPoint(   DRIVE(0)    ,FLOAT(0)     ,DRIVE(100)  ,DRIVE(0)     ),     1);
            add_sim_point(SimPoint(   DRIVE(0)    ,FLOAT(0)     ,DRIVE(0)    ,DRIVE(0)     ),     1);

            substep_count = 5000;
            connection_mask = CONMASK_NES;

            break;

        case 5:              //       N            E             S            W                  time
            add_sim_point(SimPoint(   DRIVE(0)    ,FLOAT(0)     ,DRIVE(0)    ,DRIVE(0)     ),     1);
            add_sim_point(SimPoint(   DRIVE(0)    ,FLOAT(10)    ,DRIVE(0)    ,DRIVE(20)    ),     1);
            add_sim_point(SimPoint(   DRIVE(0)    ,FLOAT(20)    ,DRIVE(0)    ,DRIVE(40)    ),     1);
            add_sim_point(SimPoint(   DRIVE(0)    ,FLOAT(30)    ,DRIVE(0)    ,DRIVE(60)    ),     1);
            add_sim_point(SimPoint(   DRIVE(0)    ,FLOAT(40)    ,DRIVE(0)    ,DRIVE(80)    ),     1);
            add_sim_point(SimPoint(   DRIVE(0)    ,FLOAT(50)    ,DRIVE(0)    ,DRIVE(100)   ),     1);
            add_sim_point(SimPoint(   DRIVE(0)    ,FLOAT(25)    ,DRIVE(0)    ,DRIVE(50)    ),     1);
            add_sim_point(SimPoint(   DRIVE(0)    ,FLOAT(50)    ,DRIVE(0)    ,DRIVE(100)   ),     1);
            add_sim_point(SimPoint(   DRIVE(0)    ,FLOAT(20)    ,DRIVE(0)    ,DRIVE(40)    ),     1);
            substep_count = 5000;
            connection_mask = CONMASK_WE;
            break;

        case 6:              //       N            E             S            W                  time
            add_sim_point(SimPoint(   DRIVE(0)    ,FLOAT(0)     ,DRIVE(0)    ,DRIVE(0)     ),     1);
            add_sim_point(SimPoint(   DRIVE(0)    ,FLOAT(20)    ,DRIVE(0)    ,DRIVE(10)    ),     1);
            add_sim_point(SimPoint(   DRIVE(0)    ,FLOAT(40)    ,DRIVE(0)    ,DRIVE(20)    ),     1);
            add_sim_point(SimPoint(   DRIVE(0)    ,FLOAT(60)    ,DRIVE(0)    ,DRIVE(30)    ),     1);
            add_sim_point(SimPoint(   DRIVE(0)    ,FLOAT(80)    ,DRIVE(0)    ,DRIVE(40)    ),     1);
            add_sim_point(SimPoint(   DRIVE(0)    ,FLOAT(100)   ,DRIVE(0)    ,DRIVE(50)    ),     1);
            add_sim_point(SimPoint(   DRIVE(0)    ,FLOAT(50)    ,DRIVE(0)    ,DRIVE(25)    ),     1);
            add_sim_point(SimPoint(   DRIVE(0)    ,FLOAT(100)   ,DRIVE(0)    ,DRIVE(50)    ),     1);
            add_sim_point(SimPoint(   DRIVE(0)    ,FLOAT(40)    ,DRIVE(0)    ,DRIVE(20)    ),     1);
            add_sim_point(SimPoint(   DRIVE(0)    ,FLOAT(40)    ,DRIVE(0)    ,DRIVE(20)    ),     1);
            add_sim_point(SimPoint(   DRIVE(0)    ,FLOAT(20)    ,DRIVE(0)    ,DRIVE(10)    ),     1);
            substep_count = 5000;
            connection_mask = CONMASK_WE;
            break;

        case 7:              //       N            E             S            W                  time
            add_sim_point(SimPoint(   DRIVE(0)    ,FLOAT(0)     ,DRIVE(0)    ,DRIVE(0)    ),     1);
            add_sim_point(SimPoint(   DRIVE(20)   ,FLOAT(20)    ,DRIVE(20)   ,DRIVE(0)    ),     1);
            add_sim_point(SimPoint(   DRIVE(40)   ,FLOAT(40)    ,DRIVE(40)   ,DRIVE(0)    ),     1);
            add_sim_point(SimPoint(   DRIVE(60)   ,FLOAT(60)    ,DRIVE(60)   ,DRIVE(0)    ),     1);
            add_sim_point(SimPoint(   DRIVE(80)   ,FLOAT(80)    ,DRIVE(80)   ,DRIVE(0)    ),     1);
            add_sim_point(SimPoint(   DRIVE(100)  ,FLOAT(100)   ,DRIVE(100)  ,DRIVE(0)    ),     1);
            add_sim_point(SimPoint(   DRIVE(0)    ,FLOAT(0)     ,DRIVE(0)    ,DRIVE(0)    ),     1);
            add_sim_point(SimPoint(   DRIVE(50)   ,FLOAT(25)    ,DRIVE(0)    ,DRIVE(0)    ),     1);
            add_sim_point(SimPoint(   DRIVE(100)  ,FLOAT(50)    ,DRIVE(0)    ,DRIVE(0)    ),     1);
            add_sim_point(SimPoint(   DRIVE(100)  ,FLOAT(75)    ,DRIVE(50)   ,DRIVE(0)    ),     1);
            add_sim_point(SimPoint(   DRIVE(100)  ,FLOAT(100)   ,DRIVE(100)  ,DRIVE(0)    ),     1);
            add_sim_point(SimPoint(   DRIVE(50)   ,FLOAT(75)    ,DRIVE(100)  ,DRIVE(0)    ),     1);
            add_sim_point(SimPoint(   DRIVE(0)    ,FLOAT(50)    ,DRIVE(100)  ,DRIVE(0)    ),     1);
            add_sim_point(SimPoint(   DRIVE(0)    ,FLOAT(25)    ,DRIVE(50)   ,DRIVE(0)    ),     1);
            add_sim_point(SimPoint(   DRIVE(0)    ,FLOAT(0)     ,DRIVE(0)    ,DRIVE(0)    ),     1);

            substep_count = 5000;
            connection_mask = CONMASK_NES;
            break;



        default:
            printf("no level %d\n", level_index);
            break;
    }
}

void Level::reset(LevelSet* level_set)
{
    sim_point_index = 0;
    substep_index = 0;

    circuit->reset(level_set);
    N=0;
    S=0;
    E=0;
    W=0;
}

void Level::advance(unsigned ticks)
{
    static int val = 0;
    for (int i = 0; i < ticks; i++)
    {
        SimPoint& simp = sim_points[sim_point_index];
        N.apply(simp.N.out_value, simp.N.out_drive);
        E.apply(simp.E.out_value, simp.E.out_drive);
        S.apply(simp.S.out_value, simp.S.out_drive);
        W.apply(simp.W.out_value, simp.W.out_drive);
        
        N.pre();
        E.pre();
        S.pre();
        W.pre();

        circuit->sim_pre(PressureAdjacent(N, E, S, W));
        circuit->sim_post(PressureAdjacent(N, E, S, W));

        N.post();
        E.post();
        S.post();
        W.post();
        
        substep_index++;
        if (substep_index >= substep_count)
        {
            substep_index = 0;
            simp.record(N.value, E.value, S.value, W.value);
//            printf("%d %d %d %d\n", pressure_as_percent(N.value), pressure_as_percent(E.value), pressure_as_percent(S.value), pressure_as_percent(W.value));
            
            sim_point_index++;
            if (sim_point_index >= sim_point_count)
                sim_point_index = 0;
        }
    }
}

LevelSet::LevelSet(SaveObject* sobj)
{
    SaveObjectList* slist = sobj->get_list();
    for (int i = 0; i < LEVEL_COUNT; i++)
    {
        if (i < slist->get_count())
            levels[i] = new Level(i, slist->get_item(i));
        else
            levels[i] = new Level(i);
    }
}

LevelSet::LevelSet()
{
        for (int i = 0; i < LEVEL_COUNT; i++)
        {
            levels[i] = new Level(i);
        }
}

SaveObject* LevelSet::save()
{
    SaveObjectList* slist = new SaveObjectList;
    
    for (int i = 0; i < LEVEL_COUNT; i++)
    {
        slist->add_item(levels[i]->save());
    }
    return slist;
}
