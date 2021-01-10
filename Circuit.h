#pragma once
#include "Misc.h"
#include "SaveState.h"

#include <SDL.h>

#define PRESSURE_SCALAR 1024


typedef int Pressure;

class CircuitPressure
{
public:
    Pressure value = 0;
    Pressure move_next = 0;
    Pressure vented = 0;
    bool touched = false;

    void move(Pressure vol)
    {
        move_next += vol;
        touched = true;
    }
    
    void vent(void)
    {
        move_next -= value / 2;
        vented += value / 2;
        if (vented)
            touched = true;
    }

    void pre(void)
    {
        vented = 0;
        touched = false;
    }
    
    void post(void)
    {
        value += move_next;
        move_next = 0;
        if (value > 100 * PRESSURE_SCALAR)
            value = 100 * PRESSURE_SCALAR;
    }
    
    CircuitPressure(Pressure value_):
        value(value_)
    {}
        CircuitPressure()
    {}

};

enum Connections
{
    CONNECTIONS_NONE,
    CONNECTIONS_NW,
    CONNECTIONS_NE,
    CONNECTIONS_NS,
    CONNECTIONS_EW,
    CONNECTIONS_ES,
    CONNECTIONS_WS,
    CONNECTIONS_NWE,
    CONNECTIONS_NES,
    CONNECTIONS_NWS,
    CONNECTIONS_EWS,
    CONNECTIONS_NS_WE,
    CONNECTIONS_NW_ES,
    CONNECTIONS_NE_WS,
    CONNECTIONS_ALL,
};

enum CircuitElementType
{
    CIRCUIT_ELEMENT_TYPE_PIPE,
    CIRCUIT_ELEMENT_TYPE_VALVE,
    CIRCUIT_ELEMENT_TYPE_SOURCE,
};
class PressureAdjacent
{
public:
    CircuitPressure& N;
    CircuitPressure& E;
    CircuitPressure& S;
    CircuitPressure& W;
    PressureAdjacent(CircuitPressure& N_, CircuitPressure& E_, CircuitPressure& S_, CircuitPressure& W_):
    N(N_),
    E(E_),
    S(S_),
    W(W_)
    {}

    PressureAdjacent(PressureAdjacent adj, Direction dir):
        N(dir == DIRECTION_N ? adj.N : dir == DIRECTION_E ? adj.E : dir == DIRECTION_S ? adj.S : adj.W),
        E(dir == DIRECTION_N ? adj.E : dir == DIRECTION_E ? adj.S : dir == DIRECTION_S ? adj.W : adj.N),
        S(dir == DIRECTION_N ? adj.S : dir == DIRECTION_E ? adj.W : dir == DIRECTION_S ? adj.N : adj.E),
        W(dir == DIRECTION_N ? adj.W : dir == DIRECTION_E ? adj.N : dir == DIRECTION_S ? adj.E : adj.S)
    {
    }
};

class CircuitElement
{
public:
    SaveObject* save();
    virtual void save(SaveObjectMap*) = 0;
    static CircuitElement* load(SaveObjectMap*);

    virtual SDL_Rect getimage(void) = 0;
    virtual void sim_pre(PressureAdjacent adj) = 0;
    virtual void sim_post(PressureAdjacent adj) = 0;
    
    virtual CircuitElementType get_type() = 0;
    virtual void extend_pipe(Connections con){assert(0);}

};

class CircuitElementPipe : public CircuitElement
{
public:
    Connections connections = CONNECTIONS_NONE;

    CircuitElementPipe(){}
    CircuitElementPipe(Connections connections_):
        connections(connections_)
        {}
    CircuitElementPipe(SaveObjectMap*);

    void save(SaveObjectMap*);
    SDL_Rect getimage(void);
    void sim_pre(PressureAdjacent adj);
    void sim_post(PressureAdjacent adj);
    CircuitElementType get_type() {return CIRCUIT_ELEMENT_TYPE_PIPE;}

    void extend_pipe(Connections con);
};



class CircuitElementValve : public CircuitElement
{
    const int capacity = 10;

    Pressure pos_charge = 0;
    Pressure neg_charge = 0;
public:
    Direction direction = DIRECTION_N;

    CircuitElementValve(){}
    CircuitElementValve(Direction direction_):
        direction(direction_)
        {}
    CircuitElementValve(SaveObjectMap*);

    void save(SaveObjectMap*);
    SDL_Rect getimage(void);
    void sim_pre(PressureAdjacent adj);
    void sim_post(PressureAdjacent adj);
    CircuitElementType get_type() {return CIRCUIT_ELEMENT_TYPE_VALVE;}
};

class CircuitElementSource : public CircuitElement
{

public:
    Direction direction = DIRECTION_N;

    CircuitElementSource(){}
    CircuitElementSource(Direction direction_):
        direction(direction_)
        {}
    CircuitElementSource(SaveObjectMap*);

    void save(SaveObjectMap*);
    SDL_Rect getimage(void);
    void sim_pre(PressureAdjacent adj);
    void sim_post(PressureAdjacent adj);
    CircuitElementType get_type() {return CIRCUIT_ELEMENT_TYPE_SOURCE;}
};


class Circuit
{
public:
    CircuitElement* elements[9][9];

    CircuitPressure connections_ns[10][10];
    CircuitPressure connections_ew[10][10];
    Circuit(SaveObjectMap* omap);
    Circuit();

    SaveObject* save();
    
    void set_element_pipe(XYPos pos, Connections con);
    void set_element_valve(XYPos pos, Direction direction);
    void set_element_source(XYPos pos, Direction direction);

    void reset(void);
    void sim_pre(PressureAdjacent);
    void sim_post(PressureAdjacent);

};
