u64 file_length(FILE* file)
{
    fseek(file, 0, SEEK_END);
    u64 tell = ftell(file);
    fseek(file, 0, SEEK_SET);
    return tell;
}

const char* load_file(const char* path)
{
    FILE* file = fopen(path, "rb");
    u64 file_size = file_length(file);
    char* data = (char*)malloc(file_size + 1);
    fread(data, 1, file_size, file);
    data[file_size] = 0;
    fclose(file);
    return data;
}

u32 load_texture(const char* path)
{
    u32 ID;
    glGenTextures(1, &ID);

    s32 width, height, nrComponents;
    unsigned char* data = stbi_load(path, &width, &height, &nrComponents, 0);

    glBindTexture(GL_TEXTURE_2D, ID);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    if (data)
    {
        GLenum format;
        switch (nrComponents)
        {
            case 1:
                format = GL_RED;
                break;
            case 3:
                format = GL_RGB;
                break;
            case 4:
                format = GL_RGBA;
                break;
            default:
                format = GL_RGB;
                break;
        }

        glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else
    {
        printf("Texture failed to load at path: %s\n", path);
    }

    stbi_image_free(data);
    return ID;
}