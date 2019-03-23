#ifndef MING3D_SHADERWRITER_H
#define MING3D_SHADERWRITER_H

#include <sstream>

namespace Ming3D
{
    // TODO: move to a separate file
    class ShaderStream
    {
    private:
        std::stringstream mStream;
        int mIndentCount = 0;

    public:
        void ClearStream();
        void AddIndent(int inCount = 1);
        void RemoveIndent(int inCount = 1);
        std::stringstream& GetStream();

        template <typename T>
        ShaderStream & operator<<(T const &value)
        {
            if (mStream.str().size() > 0)
            {
                char lastChar;
                mStream.seekg(-1, std::ios_base::end);
                lastChar = mStream.peek();
                mStream.seekg(0, std::ios_base::beg);
                if (lastChar == '\n')
                {
                    for (int i = 0; i < mIndentCount; i++)
                        mStream << "   ";
                }
            }
            mStream << value;
            return *this;
        }
    };

    class ShaderWriter
    {
    };
}

#endif
