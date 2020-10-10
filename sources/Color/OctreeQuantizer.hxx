#pragma once

#include <array>
#include <vector>
#include "Common/Concepts.hxx"
#include "Common/Tools.hxx"
#include "ColorDetail.hxx"

// XXX: the createPalette() method does not priduce proper result (color values loose to much
//      precision and the current implementation also leaks memory

namespace Color::Quantize
{
    namespace Detail
    {
        template <Concept::RGBA Color, int32_t MaxDepth> class Node;
        template <Concept::RGBA Color, int32_t MaxDepth> class Octree;

        template <Concept::RGBA Color, int32_t MaxDepth>
        using Nodes = std::vector<Node<Color,MaxDepth> *>;
        template <Concept::RGBA Color>
        using Colors = std::vector<Color>;

        template <Concept::RGBA Color>
        constexpr inline int32_t colorIndex(const Color &c, const int32_t l)
        {
            return (((c.r >> l) & 1) << 2) + (((c.g >> l) & 1) << 1) + ((c.b >> l) & 1);
        }

        template <Concept::RGBA Color, int32_t MaxDepth>
        class Node {
        public:
            //--- public types and constants ---
            using CType = decltype(Color::r);

            //--- public constructors ---
            Node(const int32_t level, Octree<Color,MaxDepth> *parent)
            : _nodes(), _color(), _count(0), _pindex()
            {
                if (level < (MaxDepth - 1))
                    parent->addLevelNode(level, this);
            }

            Node(const Node &rhs) = delete;
            Node(Node &&rhs) = delete;

            ~Node() noexcept
            {
                for (int32_t i = 0; i < 8; ++i)
                    delete _nodes[i];
            }

            //--- public operators ---
            Node &operator=(const Node &rhs) = delete;
            Node &operator=(Node &&rhs) = delete;

            //--- public methods ---
            bool isLeaf() const noexcept
            {
                return _count;
            }

            Nodes<Color,MaxDepth> leafNodes() const
            {
                Nodes<Color,MaxDepth> nodes;

                for (int32_t i = 0; i < 8; ++i)
                {
                    if (!_nodes[i])
                        continue;

                    if (!_nodes[i]->isLeaf())
                    {
                        auto lnodes = _nodes[i]->leafNodes();
                        nodes.insert(nodes.end(), lnodes.begin(), lnodes.end());
                    }
                    else
                        nodes.push_back(_nodes[i]);
                }

                return nodes;
            }

            int32_t removeLeafs()
            {
                int32_t result = 0;

                for (int32_t i = 0; i < 8; ++i)
                {
                    if (_nodes[i])
                    {
                        _color += _nodes[i]->_color;
                        _count += _nodes[i]->_count;
                        ++result;
                        delete _nodes[i];
                        _nodes[i] = nullptr;
                    }
                }

                return result - 1;
            }

            void addColor(const Color &color, const int32_t level, Octree<Color,MaxDepth> *parent)
            {
                if (level >= MaxDepth)
                {
                    _color += color;
                    ++_count;
                }
                else
                {
                    const int32_t index = colorIndex(color, level);

                    if (!_nodes[index])
                        _nodes[index] = new Node(level, parent);
                    _nodes[index]->addColor(color, level + 1, parent);
                }
            }

            Color color() const noexcept
            {
                return {static_cast<CType>(_color.r / _count),
                        static_cast<CType>(_color.g / _count),
                        static_cast<CType>(_color.b / _count),
                        std::numeric_limits<CType>::max()};
            }

            void setPaletteIndex(const int32_t val) noexcept
            {
                _pindex = val;
            }

            int32_t paletteIndex(const Color &color, const int32_t level) const noexcept
            {
                if (!isLeaf())
                {
                    if (const int32_t index = colorIndex(color, level); !_nodes[index])
                    {
                        for (int32_t i = 0; i < 8; ++i)
                        {
                            if (_nodes[i])
                                return _nodes[i]->paletteIndex(color, level + 1);
                        }
                    }
                    else
                        return _nodes[index]->paletteIndex(color, level + 1);
                }

                return _pindex;
            }

        private:
            //--- private properties ---
            std::array<Node *,8> _nodes;
            QRGBA<int32_t> _color;
            int32_t _count;
            int32_t _pindex;
        };

        template <Concept::RGBA Color, int32_t MaxDepth>
        class Octree {
        public:
            //--- public constructors ---
            Octree()
            : _levels(), _root(new Node<Color,MaxDepth>(0, this))
            {
            }

            Octree(const Octree &rhs) = delete;
            Octree(Octree &&rhs) = delete;

            ~Octree() noexcept
            {
                delete _root;
            }

            //--- public operators ---
            Octree &operator=(const Octree &rhs) = delete;
            Octree &operator=(Octree &&rhs) = delete;

            //--- public methods ---
            Nodes<Color,MaxDepth> leafNodes() const
            {
                return _root->leafNodes();
            }

            void addLevelNode(const int32_t level, Node<Color,MaxDepth> *node)
            {
                _levels[level].push_back(node);
            }

            void addColor(const Color &color)
            {
                _root->addColor(color, 0, this);
            }

            int32_t paletteIndex(const Color &color) const noexcept
            {
                return _root->paletteIndex(color, 0);
            }

            Colors<Color> createPalette(const int32_t max_colors)
            {
                Colors<Color> palette;
                int32_t pindex = 0;
                int32_t lcount = leafNodes().size();

                for (int32_t level = (MaxDepth - 1); level >= 0; --level)
                {
                    if (!_levels[level].empty())
                    {
                        for (size_t i = 0; i < _levels[level].size(); ++i)
                        {
                            if (lcount -= _levels[level][i]->removeLeafs(); lcount <= max_colors)
                                break;
                        }
                        if (lcount <= max_colors)
                            break;
                        _levels[level].clear();
                    }
                }

		auto leaf_nodes = leafNodes();
                for (size_t i = 0; i < leaf_nodes.size(); ++i)
                {
                    if (pindex >= max_colors)
                        break;
                    if (leaf_nodes[i]->isLeaf())
                        palette.push_back(leaf_nodes[i]->color());
                    leaf_nodes[i]->setPaletteIndex(pindex);
                    ++pindex;
                }

                return palette;
            }

        private:
            //--- private properties ---
            std::array<std::vector<Node<Color,MaxDepth> *>,MaxDepth> _levels;
            Node<Color,MaxDepth> *_root;
        };
    }

    template <Common::Concept::Class RGBA>
    bool octree(const int64_t width, const int64_t height, const int64_t colors,
                const std::vector<RGBA> &in_pixels, std::vector<RGBA> &out_pixels,
                std::vector<RGBA> &palette)
    {
        if ((width < 1) || (height < 1))
            return false;

        if (in_pixels.size() != static_cast<size_t>(width * height))
            return false;

        if (in_pixels.size() <= static_cast<size_t>(colors))
            return false;

        Detail::Octree<RGBA,sizeof (decltype(RGBA::r)) * 8> octree;

        for (const auto &color : in_pixels)
            octree.addColor(color);

        palette = octree.createPalette(colors);
        palette.resize(colors);

        out_pixels.resize(in_pixels.size());
        for (size_t i = 0; i < in_pixels.size(); ++i)
            out_pixels[i] = palette[Color::Detail::closestMatch(in_pixels[i], palette)];

        return true;
    }
}
