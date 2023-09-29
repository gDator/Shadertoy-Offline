#ifndef BATCH_HPP
#define BATCH_HPP
#include "Basic/Vertex.hpp"
#include "Basic/Vector.hpp"

#define MAX_TEXTURES 16     //TODO: Read from Graphics Card: gl.....
#define BATCH_SIZE 10000

namespace ycad
{

class DrawCall
{
    public:
        DrawCall(int glmode, int batch_size);
        ~DrawCall();
        int texture_id;
        int buffer_size = BATCH_SIZE;
        Vertex* vertex_buffer = nullptr;              //points to beginning of Buffer
        Vertex* p_vertex_buffer = nullptr;            //Points to next free position
        int vertex_count = 0;
        int vertex_alignment;

        // int availableSize();
        void begin();
        void flush();
        void end();
    private:
        unsigned int vao_id = 0;
        unsigned int vbo_id = 0;
        unsigned int index_id = 0;
        int mode;
};


class Batch
{   
    public:
        Batch(int mode);
        Batch(int glmode, int batch_size); //customize batch size to bigger ones
        ~Batch();
        void endBatch();
        int flushBatch();
        void newBatch();
        unsigned int texture_slots = 1;
        int setPoint(Vector3f vertex);
        int setTexture(unsigned int texture_id);
        void disableTexture();
        //TODO: extra draw call for each Texture. if buffer is full flush that batch
        //bind Texture with  glBindTextureUnit(i, texture_ids.at(i));
        //activate TExtture with glActiveTexture(GL_TEXTURE0 + i);
        DrawCall* p_active_draw_call = nullptr;
        Vector4f current_color;
        Vector3f current_normal;
        Vector2f current_tex_coords;
        // float current_tex_index;

    private:
        int findFullestSlot();  //return index of DrawCalls which has the highest vertex count;
        void executeDrawCall(int i);
        unsigned int m_white_texture;            //GL_TRAINGLES etc.
        std::array<std::tuple<int, DrawCall*>, MAX_TEXTURES> p_draw_calls; //int so we can set ID to -1: free slot
        

};

}

#endif /*BATCH_HPP*/