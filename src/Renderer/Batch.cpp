#include "Renderer/Batch.hpp"

#include <GL/glew.h>
#include <iostream>
#include "logger.hpp"
namespace ycad
{
    
DrawCall::DrawCall(int glmode, int batch_size) : mode(glmode), buffer_size(batch_size)
{
    if(vertex_buffer != nullptr)
        return;
    vertex_count = 0;
    //create Vertex Data


    //Create VAO
    glCreateVertexArrays(1, &vao_id);
    glBindVertexArray(vao_id);

    if(mode == GL_LINES)
        vertex_alignment = 2;
    else if(mode == GL_TRIANGLES)
        vertex_alignment = 3;
    else if(mode == GL_QUADS)
        vertex_alignment = 4;
    else if(mode == GL_POINT)
        vertex_alignment = 1;
    else
        vertex_alignment = 1;
    
    
    uint32_t indices[buffer_size*6];
    int k = 0;
    //Indices can be precalculated for specifid modes
    for(int i = 0; i < buffer_size*6; i += 6)
    {
        // std::cout << std::endl;
        indices[i] = 4*k;
        indices[i + 1] = 4*k + 1;
        indices[i + 2] = 4*k + 2;
        indices[i + 3] = 4*k + 2;
        indices[i + 4] = 4*k + 3;
        indices[i + 5] = 4*k;
        // for(int j = 0; j < 6; j++)
        //     std::cout << indices[j] <<", ";
        // std::cout << std::endl;
        k++;
    }
    
    //make sure for a specific mode that memory has a size multiple of the mode (point 1, lline 2, triangle 3 , quad 4)
    //in theory u can adjust flush Batch, so that overhang vertecies get written to next batch draw call
    //this is much simpler and less computational
    // buffer_size = buffer_size - buffer_size%vertex_alignment; 
    vertex_buffer = new Vertex[vertex_alignment*buffer_size];
    p_vertex_buffer = vertex_buffer; //acess pointer to beginning

    //Index Buffer
    glGenBuffers(1, &index_id);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, index_id);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), &indices[0], GL_STATIC_DRAW);
    
    //Create VBOs 
    glGenBuffers(1, &vbo_id);
    glBindBuffer(GL_ARRAY_BUFFER, vbo_id);
    glBufferData(GL_ARRAY_BUFFER, vertex_alignment*buffer_size*sizeof(Vertex), nullptr, GL_DYNAMIC_DRAW); //only allocate data

    glEnableVertexArrayAttrib(vao_id, 0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)(0 * sizeof(GLfloat))); // (GLvoid*)(6 * sizeof(GLfloat)
    glEnableVertexArrayAttrib(vao_id, 1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)(3 * sizeof(GLfloat)));
    glEnableVertexArrayAttrib(vao_id, 2);
    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)(5 * sizeof(GLfloat)));
    glEnableVertexArrayAttrib(vao_id, 3);
    glVertexAttribPointer(3, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)(8 * sizeof(GLfloat))); 
    // glEnableVertexArrayAttrib(vao_id, 4);
    // glVertexAttribPointer(4, 1, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const void*)offsetof(Vertex, tex_index));
    
    glBindVertexArray(0);
}


DrawCall::~DrawCall()
{
    //Destroy Data from Batch
    glDeleteVertexArrays(1, &vao_id);
    glDeleteBuffers(1, &vbo_id);
    // index buffer only available if we draw as quads
    glDeleteBuffers(1, &index_id);

    delete[] vertex_buffer;
}

// int DrawCall::availableSize()
// {
//     return (buffer_size - vertex_count)/vertex_alignment;
// }

void DrawCall::begin()
{
    p_vertex_buffer = vertex_buffer; //jump to beginning
}

void DrawCall::flush()
{
        // for(int i = 0; i < texture_slots; i++)
    //     glBindTextureUnit(i, texture_ids.at(i));    
    // std::cout << "Flush: " << index_count << std::endl;
    // std::cout << "bind texture: " << texture_ids.at(0) << std::endl;
    // glActiveTexture(GL_TEXTURE0);
    // glBindTexture(GL_TEXTURE_2D, texture_ids.at(0));
    glBindVertexArray(vao_id);
    if(mode == GL_LINES && vertex_count%2 == 0)
    {
        glDrawArrays(GL_LINES, 0, vertex_count); //glDrawArrays(GL_TRIANGLES, 0, vertex_count);
    }
    else if(mode == GL_TRIANGLES && vertex_count%3 == 0)
    {
        //glDrawElements(GL_TRIANGLES, vertex_count, GL_UNSIGNED_INT, 0); //this in combination with wrong indices calculation caused two days of trouble
        glDrawArrays(GL_TRIANGLES, 0, vertex_count);
    }
    else if(mode == GL_POINTS)
    {
        glDrawArrays(GL_POINTS, 0, vertex_count);
    }
    else if(mode == GL_QUADS && vertex_count%4 == 0) //4 vertecies per quad to ensure veretx_count/4*6 is an integer
    {
        glDrawElements(GL_TRIANGLES, vertex_count/4*6, GL_UNSIGNED_INT, 0); //4 vertecies build one quad. 6 indices are needed to build a quad: 1,2,3 2, 1
    }
    else
    {
        LOG_ERROR("UNKNOWN MODE OR BAD VERTEX COUNT" << mode << ": " << vertex_count);
        assert(0!=0);
    }

    vertex_count = 0;
    glBindVertexArray(0);
}

void DrawCall::end()
{
    GLsizeiptr size = (uint8_t*)p_vertex_buffer - (uint8_t*)vertex_buffer;
    glBindBuffer(GL_ARRAY_BUFFER, vbo_id);
    glBufferSubData(GL_ARRAY_BUFFER, 0, size, vertex_buffer);
}

Batch::Batch(int glmode) : Batch(glmode, BATCH_SIZE) {}
Batch::Batch(int glmode, int batch_size) 
{
        //White Texture for Colormixing
    glCreateTextures(GL_TEXTURE_2D, 1, &m_white_texture);
    glBindTexture(GL_TEXTURE_2D, m_white_texture);
    
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    uint32_t color = 0xffffffff;
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, 1, 1, 0, GL_RGBA, GL_UNSIGNED_BYTE, &color);


    for(auto&& [id, draw_call] : p_draw_calls)
    {
        id = -1; //mark all slots as free
        draw_call = new DrawCall(glmode, batch_size);
    }
    texture_slots = 1;
    std::get<0>(p_draw_calls[0]) = m_white_texture; //set slot 0 to texture 
    disableTexture(); 
}

Batch::~Batch()
{
    glDeleteTextures(1, &m_white_texture);
    for(auto&& [texture_id, draw_call] : p_draw_calls)  //free all Draw_Calls!
        delete draw_call;
}

void Batch::endBatch()
{
    for(auto&& [texture_id, draw_call] : p_draw_calls)
        draw_call->end();
        
}

int Batch::flushBatch()
{
    int ts = 0, flush_count = 0;
    for(auto&& [texture_id, draw_call] : p_draw_calls)
    {
        //could use execuet DrawCall but its unnecessary effort
        if(texture_id != -1) // reduce actual draw call;
        {
            glActiveTexture(GL_TEXTURE0); //glActiveTexture(GL_TEXTURE0 + ts);
            glBindTexture(GL_TEXTURE_2D, texture_id); //bind texture to unit
            draw_call->flush();
            flush_count++;
        }
        ts++;
    }    

    return flush_count;      //Return amount of DrawCalls
}

void Batch::newBatch()
{
    for(int i = 0; i < p_draw_calls.size(); i++)
    {
        std::get<1>(p_draw_calls[i])->begin();
        std::get<0>(p_draw_calls[i]) = -1; 
    }
    std::get<0>(p_draw_calls[0]) = m_white_texture; //set slot 0 to texture 
    texture_slots = 1; // free all texture slot
}

int Batch::setPoint(Vector3f vertex)
{
    assert(p_active_draw_call != nullptr);
        //actuallly a vertex with current settings
    int needs_flush = 0;
    if(p_active_draw_call->vertex_count + p_active_draw_call->vertex_alignment >= p_active_draw_call->buffer_size*p_active_draw_call->vertex_alignment) //check also if vertex count is even number raylib rlgl z.1401
    {
        assert(p_active_draw_call->vertex_count%p_active_draw_call->vertex_alignment == 0);
        int index, texture;
        // std::cout << "batch full: " << p_active_batch->availableSize() << ", " <<p_active_batch->index_count <<"," << p_active_batch->max_index_count <<std::endl;

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texture); //bind texture to unit
        
        p_active_draw_call->end();
        p_active_draw_call->flush();
        // std::cout << "buffer was full: " << texture << ", " << p_active_draw_call->vertex_count << std::endl;
        p_active_draw_call->begin();
        
        needs_flush++;
    }
    p_active_draw_call->p_vertex_buffer->position = vertex;
    p_active_draw_call->p_vertex_buffer->color = current_color;
    p_active_draw_call->p_vertex_buffer->normal = current_normal;
    p_active_draw_call->p_vertex_buffer->tex_coords = current_tex_coords;
    // a.tex_index = p_active_batch->current_tex_index;
    p_active_draw_call->p_vertex_buffer++;
    p_active_draw_call->vertex_count++;//= p_active_batch->vertex_alignment; //<--this makes sure draw calls always are in right package size, but produces a lot of overhang (aka draw stuff from memory where nothing was stored)
    return needs_flush;
}

int Batch::setTexture(unsigned int texture_id)
{
    assert(p_active_draw_call != nullptr);
    assert(p_active_draw_call->vertex_count%p_active_draw_call->vertex_alignment == 0); // dont activate texture when primitve is not finished
    int needs_flush = 0;
    //check for existance
    int pos = 0;
    for(auto&& [id, draw_call] : p_draw_calls)
    {
        if((int)texture_id == id) //found texture
        {
            // std::cout << "texture found: " << id << "at" << pos<<std::endl;
            p_active_draw_call = draw_call; //set draw call to texture specific draw call
            return needs_flush;
        }
        pos++;
    }
    if(texture_slots + 1 > MAX_TEXTURES)    //Textures full, flush drawCall with max vertex_count
    {        
        executeDrawCall(findFullestSlot());

        needs_flush++;
    }
    for(auto&& [id, draw_call] : p_draw_calls)
    {
        if(id == -1) //found free slot
        {
            // std::cout << "found free slot, texture:" << id << std::endl;
            id = texture_id; //bind texture to this slot
            p_active_draw_call = draw_call;
            texture_slots++;
            return needs_flush;
        }
    }
    assert(0!=0);
    return needs_flush;
}

void Batch::disableTexture()
{
    p_active_draw_call = std::get<1>(p_draw_calls[0]); //write vertices into slot 0
}

int Batch::findFullestSlot()
{
    int max_vertex_count = 0;
    int index = 1;
    for(int i = 1; i < p_draw_calls.size(); i++) //slot 0 is always white texture
    {
        int count = std::get<1>(p_draw_calls[i])->vertex_count;
        if(count > max_vertex_count)
        {
            index = i;
            max_vertex_count = count;
        }
    }

    // std::cout << "Max count: " <<max_vertex_count << "at " << index << std::endl;
    return index;
}

void Batch::executeDrawCall(int i)
{
    assert(i > 0);
    assert(i < p_draw_calls.size());
    assert(p_active_draw_call->vertex_count%p_active_draw_call->vertex_alignment == 0); // cant flush if primitive is not complete
    DrawCall* draw_call = std::get<1>(p_draw_calls[i]);
    int& ts = std::get<0>(p_draw_calls[i]);
    assert(ts != -1); //
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, ts); //bind texture to unit
    //flsuh this one slot
    draw_call->end();
    draw_call->flush();
    draw_call->begin();
    ts = -1; //mark slot as free
    texture_slots--;
}

}//namespace ycad