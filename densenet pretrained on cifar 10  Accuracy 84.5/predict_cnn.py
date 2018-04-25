import cnn
import numpy as np
import pickle
from PIL import Image


def save_model(model,filename):
    with open(filename, 'wb') as f:
        pickle.dump(model, f)

def load_model(filename):
    pickle_in = open(filename, 'rb')
    return pickle.load(pickle_in)

def load_encoding_model(filename,encode='ascii'):
    pickle_in = open(filename, 'rb')
    return pickle.load(pickle_in,encoding=encode)

def convert_to_onehot(y_,n_classes):
    y_ = y_.reshape(len(y_))
    return np.eye(n_classes)[np.array(y_, dtype=np.int32)]

def resize_image(img, width=80, length=80):
    img=Image.fromarray(img)
    img=img.resize((width,length), Image.ANTIALIAS)
    return np.array(img)

def read_image(filename):
    img = Image.open(filename)
    arr = np.array(img)
    return arr

mean=120.707565124
std=64.1500758911



text_labels=['airplane', 'automobile', 'bird', 'cat', 'deer', 'dog', 'frog', 'horse', 'ship', 'truck']
print(text_labels)

##  setup pretrained model  (Run only once)
model=cnn.cnn()
model.batch_size=20
model.epochs=200
model.image_shape=[32,32,3]
model.learning_rate=0.001
model.no_of_classes=10
model.working_dir='cnn_cifar'
model.model_restore=True
model.setup()

#############################################################################
# reading of one image
image=read_image('c.png')
image=resize_image(image,32,32) ## image has a shape 32*32*3

# normalizing a image
image=(image-mean)/std

# setup data for insertion to the model 
test={}
test['x']=np.reshape(image, [-1,32,32,3])
pred=model.predict(test)

# pred is list of prediction
print('prediction : ',text_labels[pred[0]])

#############################################################################
#in case of multiple images

file_path=['h.jpg','c.png','t.jpeg','t1.jpeg','b.jpg']

images=[]
result_path=[]
for p in file_path:
    try:
        image=read_image(p)
        image=resize_image(image,32,32)
        images.append(image)
        result_path.append(p)
    except:
        print("error reading in image file")
# concatenate a images to make a 4D numpy array
images=np.concatenate(images,0).reshape([-1, 32, 32,3])

images=(images-mean)/std

test={}
test['x']=images
pred=model.predict(test)

for i in range(len(pred)):
    print('prediction of ',result_path[i],' is ',text_labels[pred[i]])

# clear memory not necessory but better if you use it
model.clear()
