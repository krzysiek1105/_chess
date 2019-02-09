#ifndef __POSITION_H__
#define __POSITION_H__

#include <cstdint>
#include <string>
#include <string.h>
#include <stdexcept>
#include <ctype.h>

class Position
{
  private:
    int8_t row, col;

    inline bool set(const char *position)
    {
        if (strlen(position) < 2)
            return false;

        int16_t col = tolower(position[0]) - 'a';
        int16_t row = tolower(position[1]) - '1';
        if (row < 0 || row > 7 || col < 0 || col > 7)
            return false;

        this->row = row;
        this->col = col;
        return true;
    }

    inline bool set(std::string position)
    {
        if (position.length() < 2)
            return false;

        int16_t col = tolower(position[0]) - 'a';
        int16_t row = tolower(position[1]) - '1';
        if (row < 0 || row > 7 || col < 0 || col > 7)
            return false;

        this->row = row;
        this->col = col;
        return true;
    }

    inline bool set(int32_t row, int32_t col)
    {
        if (row < 0 || row > 7 || col < 0 || col > 7)
            return false;

        this->row = row;
        this->col = col;
        return true;
    }

  public:
    Position(const char *position)
    {
        if (!set(position))
            throw std::invalid_argument("The position of the piece is out of range.");
    }

    Position(std::string &position)
    {
        if (!set(position))
            throw std::invalid_argument("The position of the piece is out of range.");
    }

    Position(int32_t row, int32_t col)
    {
        if (!set(row, col))
            throw std::invalid_argument("The position of the piece is out of range.");
    }

    std::string toString() const
    {
        std::string out;
        out += "abcdefgh"[this->col];
        out += "12345678"[this->row];
        return out;
    }

    bool operator==(const Position &position) const
    {
        return (this->row == position.row && this->col == position.col);
    }

    bool operator==(const char *position) const
    {
        return (this->row == (position[1] - '1')) && (this->col == (position[0] - 'a'));
    }

    bool operator==(std::string position) const
    {
        return (this->row == (position[1] - '1')) && (this->col == (position[0] - 'a'));
    }

    Position &operator=(const Position &position)
    {
        if (!this->set(position.row, position.col))
            throw std::invalid_argument("The position of the piece is out of range.");
        return *this;
    }

    Position &operator=(const char *position)
    {
        if (!this->set(position))
            throw std::invalid_argument("The position of the piece is out of range.");
        return *this;
    }

    Position &operator=(std::string position)
    {
        if (!this->set(position))
            throw std::invalid_argument("The position of the piece is out of range.");
        return *this;
    }
};

namespace std
{
template <>
struct hash<Position>
{
    std::size_t operator()(const Position &p) const noexcept
    {
        return std::hash<std::string>{}(p.toString());
    }
};
} // namespace std

#endif //__POSITION_H__