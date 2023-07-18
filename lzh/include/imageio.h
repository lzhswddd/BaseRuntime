#ifndef IMAGEIO_H
#define IMAGEIO_H

#include <string>
#include <iostream>

template<typename ImageType>
class ImageIOSupprt
{
public:
    ImageIOSupprt(){}
    ImageIOSupprt(ImageType *image):image(image){}
    ~ImageIOSupprt(){
        if(alloc&&image){
            delete image;
            image = nullptr;
        }
    }
    virtual bool Support()const{return true;}
    virtual int LimitWidth()const{return limitWidth;}
    virtual int LimitHeight()const{return limitHeight;}
    virtual void SetLimitWidth(int limitWidth){this->limitWidth=limitWidth;}
    virtual void SetLimitHeight(int limitHeight){this->limitHeight=limitHeight;}
    void SetImage(const ImageType *image){this->image=image;}
    ImageType& GetImage(){return *image;}
    const ImageType& GetImage()const{return *image;}
    virtual void CopyPartStart(){}
    virtual void CopyPartEnd(){}
protected:
    int limitWidth = 10000;
    int limitHeight = 10000;
    ImageType *image = nullptr;
    bool alloc = false;

    void AllocImage(){if(image==nullptr){image = new ImageType;alloc=true;}}
private:
    ImageIOSupprt(const ImageIOSupprt&){}
    ImageIOSupprt& operator = (const ImageIOSupprt<ImageType>&){return *this;}
};

template<typename _Tp>
class ImageIODevice : public ImageIOSupprt<_Tp>
{
public:
    using ImageType = _Tp;
    ImageIODevice():ImageIOSupprt<_Tp>(){}
    ImageIODevice(ImageType *image):ImageIOSupprt<_Tp>(image){}
    bool Support()override{return false;}
    void GetSize(int& width, int& height){width=height=0;}
    void GetChannel(int& channel){channel=0;}
    void GenSize(int width, int height, int channel){width=height=channel=0;}
    void CopyPart(const ImageType&, int x, int y, int width, int height) {x=y=width=height=0;}
    void CropPart(ImageType&, int x, int y, int width, int height) {x=y=width=height=0;}
    static bool deserialize(const std::string &, ImageType&) {return false;}
    static bool serialize(const std::string &, const ImageType&) {return false;}
};
class ImageIOPath
{
public:
    ImageIOPath(){}
    ImageIOPath(const char* filePath):filePath(filePath){init();}
    ImageIOPath(const std::string & filePath):filePath(filePath){init();}
    ImageIOPath(const std::string & filePath, const std::string & suffix):filePath(filePath), suffix(suffix){}
    std::string filePath;
    std::string suffix = ".jpg";

protected:
    void init(){
        if(filePath.size() >= 4){
            auto idx = filePath.rfind('.');
            if(std::string::npos != idx){
                std::string _suffix = filePath.substr(idx);
                suffix = _suffix;
                filePath = filePath.substr(0, idx);
            }
        }
    }
};

class ImageIO
{
public:
    static std::string GenName(int row, int col, int rows, int cols, size_t width, size_t height, ImageIOPath path){
        path.filePath = path.filePath + "_";
        std::string mark =
                  "X" + std::to_string(col)
                + "Y" + std::to_string(row)
                + "R" + std::to_string(rows)
                + "C" + std::to_string(cols)
                + "W" + std::to_string(width)
                + "H" + std::to_string(height);
//        std::string encoded = Base64::Encode(mark.c_str(),(int)mark.size());
        std::string imagePath = path.filePath + mark + path.suffix;
        return imagePath;
    }
    static std::string GenName(ImageIOPath path){
        std::string imagePath = path.filePath+path.suffix;
        return imagePath;
    }
    template<typename ImageType>
    static bool Save(ImageIOPath path, const ImageType &image, std::string *filename = nullptr)
    {
        ImageIODevice<ImageType> device((ImageType*)&image);
        return Save(path, &device, filename);
    }
    template<typename ImageType>
    static bool Save(ImageIOPath path, ImageIODevice<ImageType> *device, std::string *filename = nullptr)
    {
        if(path.filePath.empty())return false;
        if(device==nullptr)return false;
        if(!device->Support())return false;
        int limitWidth = device->LimitWidth();
        int limitHeight = device->LimitHeight();
        if(limitHeight==0||limitWidth==0)return false;
        bool haveSuffix = false;
        if(path.filePath.size() < 4)
            haveSuffix = true;
        else{
            std::string str = path.filePath.substr(path.filePath.size()-4,4);
            if(str == path.suffix){
                path.filePath = str;
            }
        }
        int imageWidth=0,imageHeight=0;
        device->GetSize(imageWidth, imageHeight);
        if(imageWidth == 0 || imageHeight == 0)return false;
        int xnum = imageWidth/limitWidth;
        int ynum = imageHeight/limitHeight;
        int surplusWidth = imageWidth - xnum*limitWidth;
        int surplusHeight = imageHeight - ynum*limitHeight;
        int row = 0;
        int col = 0;
        int rows = (surplusHeight>0)?ynum+1:ynum;
        int cols = (surplusWidth>0)?xnum+1:xnum;
        ImageType part;
        if(xnum==0&&ynum==0){
            return ImageIODevice<ImageType>::serialize(GenName(path), device->GetImage());
        }else if(xnum==0){
            for(row=0;row<ynum;row++){
                device->CropPart(part, col*limitWidth, row*limitHeight, imageWidth, limitHeight);
                if(!ImageIODevice<ImageType>::serialize(GenName(row,col,rows,cols,imageWidth,imageHeight,path), part))
                    return false;
            }
        }else if(ynum==0){
            for(col=0;col<xnum;col++){
                device->CropPart(part, col*limitWidth, row*limitHeight, limitWidth, imageHeight);
                if(!ImageIODevice<ImageType>::serialize(GenName(row,col,rows,cols,imageWidth,imageHeight,path), part))
                    return false;
            }
        }else{
            for(row=0;row<ynum;row++){
                for(col=0;col<xnum;col++){
                    device->CropPart(part, col*limitWidth, row*limitHeight, limitWidth, limitHeight);
                    if(!ImageIODevice<ImageType>::serialize(GenName(row,col,rows,cols,imageWidth,imageHeight,path), part))
                        return false;
                }
            }
        }
        if(surplusWidth!=0){
            for(row=0;row<ynum;row++){
                device->CropPart(part, col*limitWidth, row*limitHeight, surplusWidth, limitHeight);
                if(!ImageIODevice<ImageType>::serialize(GenName(row,col,rows,cols,imageWidth,imageHeight,path), part))
                    return false;
            }
        }
        if(surplusHeight!=0){
            for(col=0;col<xnum;col++){
                device->CropPart(part, col*limitWidth, row*limitHeight, limitWidth, surplusHeight);
                if(!ImageIODevice<ImageType>::serialize(GenName(row,col,rows,cols,imageWidth,imageHeight,path), part))
                    return false;
            }
        }
        if(surplusWidth!=0 && surplusHeight!=0){
            device->CropPart(part, col*limitWidth, row*limitHeight, surplusWidth, surplusHeight);
            if(!ImageIODevice<ImageType>::serialize(GenName(row,col,rows,cols,imageWidth,imageHeight,path), part))
                return false;
        }
        if(filename!=nullptr)
            *filename=GenName(0,0,rows,cols,imageWidth,imageHeight,path);
        return true;
    }
    template<typename ImageType>
    static bool Load(std::string path, ImageType &image)
    {
        ImageIODevice<ImageType> device(&image);
        return Load(path, &device);
    }
    template<typename ImageType>
    static bool Load(std::string path, ImageIODevice<ImageType> *device)
    {
        if(path.empty())return false;
        if(device==nullptr)return false;
        if(!device->Support())return false;
        auto start = path.rfind('_');
        auto end = path.rfind('.');
        if(start==std::string::npos||end==std::string::npos||end<start){
            return ImageIODevice<ImageType>::deserialize(path, device->GetImage());
        }else{
            std::string mark = path.substr(start+1, end-start-1);
            std::string filePath = path.substr(0, start+1);
            std::string suffix = path.substr(end);
            int x,y,rows,cols,width,height,channels;
            {
                ImageType first;
                ImageIODevice<ImageType>::deserialize(path, first);
                ImageIODevice<ImageType> firstDev(&first);
                firstDev.GetChannel(channels);
            }
            if(extractMark(mark,x,y,rows,cols,width,height)){
                int totalWidth = 0, totalHeight = 0;
                int w=0,h=0;
                device->GenSize(width, height, channels);
                device->CopyPartStart();
                for(int y=0;y<rows;y++){
                    for(int x=0;x<cols;x++){
                        ImageType part;
                        std::string code =
                                "X" + std::to_string(x) +
                                "Y" + std::to_string(y) +
                                "R" + std::to_string(rows) +
                                "C" + std::to_string(cols) +
                                "W" + std::to_string(width) +
                                "H" + std::to_string(height);
//                        std::string decoded = Base64::Decode(code.c_str(),(int)code.size());
                        std::string imagePath = filePath + code + suffix;
                        if(ImageIODevice<ImageType>::deserialize(imagePath, part)){
                            ImageIODevice<ImageType> partDev(&part);
                            partDev.GetSize(w,h);
                            if(w==0||h==0)return false;
                            device->CopyPart(part, totalWidth, totalHeight, w, h);
                            totalWidth += w;
                        }
                    }
                    totalWidth=0;
                    totalHeight += h;
                }
                device->CopyPartEnd();
            }else return false;
        }
        return true;
    }
    static bool extractMark(std::string mark, int &x, int &y, int &rows, int &cols, int &width, int &height)
    {
        auto x_idx = mark.find('X');
        auto y_idx = mark.find('Y');
        auto rows_idx = mark.find('R');
        auto cols_idx = mark.find('C');
        auto width_idx = mark.find('W');
        auto height_idx = mark.find('H');
        if(x_idx==std::string::npos||y_idx==std::string::npos||rows_idx==std::string::npos||cols_idx==std::string::npos||width_idx==std::string::npos||height_idx==std::string::npos)return false;
        if(!(x_idx<y_idx&&y_idx<rows_idx&&rows_idx<cols_idx&&cols_idx<width_idx&&width_idx<height_idx&&height_idx>x_idx))return false;
        std::string x_str = mark.substr(x_idx+1, y_idx-x_idx-1);
        std::string y_str = mark.substr(y_idx+1, rows_idx-y_idx-1);
        std::string rows_str = mark.substr(rows_idx+1, cols_idx-rows_idx-1);
        std::string cols_str = mark.substr(cols_idx+1, width_idx-cols_idx-1);
        std::string width_str = mark.substr(width_idx+1, height_idx-width_idx-1);
        std::string height_str = mark.substr(height_idx+1);
        if(x_str.empty()||y_str.empty()||rows_str.empty()||cols_str.empty()||width_str.empty()||height_str.empty())return false;
        x = std::stoi(x_str);
        y = std::stoi(y_str);
        rows = std::stoi(rows_str);
        cols = std::stoi(cols_str);
        width = std::stoi(width_str);
        height = std::stoi(height_str);
        return true;
    }
};

#endif // IMAGEIO_H
