#include <iostream>
#include <limits>
#include <map>
#include <string>
#include <vector>
#include "LZW16.hxx"

namespace Compression
{
    //--- internal stuff ---

    static const uint16_t Max = std::numeric_limits<uint16_t>::max();

    class Dictionary {
    public:
        //--- public types and constants ---
        struct Node {
            uint16_t first, left, right;
            uint8_t symbol;

            Node(const uint8_t sym)
            : first(Max), left(Max), right(Max), symbol(sym)
            {
            }
        };

        //--- public constructors ---
        Dictionary()
        : _nodes()
        {
            _nodes.reserve(Max);
            reset();
        }

        Dictionary(const Dictionary &rhs) = delete;
        Dictionary(Dictionary &&rhs) = delete;

        ~Dictionary()
        {
        }

        //--- public constructors ---
        Dictionary &operator=(const Dictionary &rhs) = delete;
        Dictionary &operator=(Dictionary &&rhs) = delete;

        //--- public methods ---
        void reset()
        {
            _nodes.clear();
            for (uint16_t i = 0; i < 256; ++i)
                _nodes.push_back(Node(i));
        }

        uint16_t insert(const uint16_t code, const uint8_t symbol)
        {
            if (_nodes.size() == Max)
                reset();

            if (code == Max)
                return symbol;

            const uint16_t nodes_size = _nodes.size();
            uint16_t code_index = _nodes[code].first;

            if (code_index != Max)
            {
                while (true)
                {
                    if (symbol < _nodes[code_index].symbol)
                    {
                        if (_nodes[code_index].left == Max)
                        {
                            _nodes[code_index].left = nodes_size;
                            break;
                        }
                        else
                            code_index = _nodes[code_index].left;
                    }
                    else if (symbol > _nodes[code_index].symbol)
                    {
                        if (_nodes[code_index].right == Max)
                        {
                            _nodes[code_index].right = nodes_size;
                            break;
                        }
                        else
                            code_index = _nodes[code_index].right;
                    }
                    else
                        return code_index;
                }
            }
            else
                _nodes[code].first = nodes_size;

            _nodes.push_back(Node(symbol));

            return Max;
        }

    private:
        //--- private properties ---
        // XXX: rethink this, may be a bottleneck
        std::vector<Node> _nodes;
    };

    //--- public constructors ---

    LZW16::LZW16()
    : _bytes_in(0), _bytes_out(0), _done(false)
    {
    }

    LZW16::~LZW16()
    {
    }

    //--- public methods ---

    uint64_t LZW16::bytesRead() const
    {
        return _bytes_in;
    }

    uint64_t LZW16::bytesWritten() const
    {
        return _bytes_out;
    }

    void LZW16::reset()
    {
        _bytes_in = 0;
        _bytes_out = 0;
        _done = false;
    }

    bool LZW16::done() const
    {
        return _done;
    }

    void LZW16::encode(std::istream &is, std::ostream &os)
    {
        Dictionary dict;
        uint16_t code = Max;
        union {
            char chr;
            uint8_t val;
        } symbol;

        while (is.get(symbol.chr))
        {
            const uint16_t tmp = code;

            ++_bytes_in;
            if ((code = dict.insert(tmp, symbol.val)) == Max)
            {
                os.write(reinterpret_cast<const char *>(&tmp), sizeof (tmp));
                code = symbol.val;
                _bytes_out += 2;
            }
        }
        if (code != Max)
        {
            os.write(reinterpret_cast<const char *>(&code), sizeof (code));
            _bytes_out += 2;
        }

        _done = true;
    }

    void LZW16::decode(std::istream &is, std::ostream &os)
    {
        std::map<uint16_t, std::string> dict;
        std::string str;
        uint16_t code;
        auto reset2 = [&]()
        {
            dict.clear();
            for (uint16_t i = 0; i < 256; ++i)
                dict[i] = static_cast<char>(i);
        };

        reset2();
        while (is.read(reinterpret_cast<char *>(&code), sizeof (code)))
        {
            _bytes_in += 2;

            if (dict.size() >= Max)
                reset2();

            if (code == dict.size())
                dict[code] = str + str.front();
            else if (!str.empty())
                dict[dict.size()] = str + dict[code].front();

            os.write(dict[code].c_str(), dict[code].size());
            _bytes_out += dict[code].size();
            str = dict[code];
        }

        _done = true;
    }
}
