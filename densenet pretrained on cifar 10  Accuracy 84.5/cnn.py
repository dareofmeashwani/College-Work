import tensorflow as tf
import numpy as np
import random
#import string
#lower=string.lower
lower = str.lower
class cnn:
    image_shape = None
    learning_rate = 0.001
    model_restore = False
    working_dir = None
    batch_size = 64
    epochs = 10
    dropout = .7
    test_result = None
    train_result = None

    no_of_classes = None
    cnn_type = 'mydensenet'
    dropout_rate=0.7

    loss_type = 'softmax'
    regularization_type = None
    regularization_coefficient = 0.0001
    logits=None

    def __init__(self):
        return

    def setup(self):
        tf.reset_default_graph()
        self.x = tf.placeholder(dtype=tf.float32,
                                shape=[None, self.image_shape[0], self.image_shape[1], self.image_shape[2]],
                                name="input")
        self.y = tf.placeholder(dtype=tf.float32, shape=[None, self.no_of_classes], name="labels")
        self.lr = tf.placeholder("float", shape=[])
        self.is_train = tf.placeholder(tf.bool, shape=[])

        if self.cnn_type == 'mydensenet':
            self.logits = self.mydensenet(self.x,self.is_train)
        elif self.cnn_type == 'densenet':
            self.logits = self.densenet(self.x,self.is_train)
        elif self.cnn_type == 'densenet121':
            self.logits=self.densenet121(self.x,self.is_train)
        elif self.cnn_type == 'densenet161':
            self.logits = self.densenet161(self.x, self.is_train)
        elif self.cnn_type == 'densenet169':
            self.logits = self.densenet169(self.x, self.is_train)
        elif self.cnn_type == 'resnet':
            self.logits = self.resnet(self.x, self.is_train)
        elif self.cnn_type == 'resnet18':
            self.logits = self.resnet18(self.x, self.is_train)
        elif self.cnn_type == 'resnet32':
            self.logits = self.resnet32(self.x, self.is_train)
        elif self.cnn_type == 'resnet50':
            self.logits = self.resnet50(self.x, self.is_train)
        elif self.cnn_type == 'resnet101':
            self.logits = self.resnet101(self.x, self.is_train)
        elif self.cnn_type == 'resnet152':
            self.logits = self.resnet152(self.x, self.is_train)

        with tf.name_scope('Output'):
            self.cross_entropy = self.get_loss(self.logits, self.y, self.loss_type)
            if self.regularization_type != None:
                self.cross_entropy = self.get_regularization(self.cross_entropy, self.regularization_type,
                                                            self.regularization_coefficient)
            hypothesis = tf.nn.softmax(self.logits, name="softmax")
            self.prediction = tf.argmax(hypothesis, 1, name='Prediction')
            correct_prediction = tf.equal(self.prediction, tf.argmax(self.y, 1), name='Correct_prediction')
            self.accuracy = tf.reduce_mean(tf.cast(correct_prediction, tf.float32), name='Accuracy')
            tf.summary.scalar("Cross_Entropy", self.cross_entropy)
            tf.summary.scalar("Accuracy", self.accuracy)

        with tf.name_scope('Optimizer'):
            # learningRate = tf.train.exponential_decay(learning_rate=learning_rate, global_step=1,
            #                                          decay_steps=shape[0], decay_rate=0.97, staircase=True,
            #                                          name='Learning_Rate')
            # optimizer = tf.train.GradientDescentOptimizer(learning_rate).minimize(cross_entropy)
            # optimizer = tf.train.MomentumOptimizer(lr, .9, use_nesterov=True).minimize(cross_entropy)
            self.optimizer = tf.train.AdamOptimizer(self.lr).minimize(self.cross_entropy)
            # optimizer = tf.train.AdadeltaOptimizer(lr).minimize(cross_entropy)
        self.session = tf.InteractiveSession()
        return

    def clear(self):
        tf.reset_default_graph()
        self.session.close()

    def leaky_relu(self, node, parameter=0.1):
        shape = node.get_shape().as_list()[1:]
        const = tf.constant(value=parameter, shape=shape)
        return tf.maximum(node * const, node)

    def get_activation_function(self, input, choice=2):
        if choice == 0 or lower(str(choice)) == 'none':
            return input
        if choice == 1 or lower(str(choice)) == 'relu':
            return tf.nn.relu(input)
        if choice == 2 or lower(str(choice)) == 'leaky_relu':
            return self.leaky_relu(input)
        if choice == 3 or lower(str(choice)) == 'crelu':
            return tf.nn.crelu(input)
        if choice == 4 or lower(str(choice)) == 'relu6':
            return tf.nn.relu6(input)
        if choice == 5 or lower(str(choice)) == 'elu':
            return tf.nn.elu(input)
        if choice == 6 or lower(str(choice)) == 'sigmoid':
            return tf.nn.sigmoid(input)
        if choice == 7 or lower(str(choice)) == 'tanh':
            return tf.nn.tanh(input)
        if choice == 8 or lower(str(choice)) == 'softplus':
            return tf.nn.softplus(input)
        if choice == 9 or lower(str(choice)) == 'softsign':
            return tf.nn.softsign(input)
        if choice == 10 or lower(str(choice)) == 'softmax':
            return tf.nn.softmax(logits=input)
        if choice == 11 or lower(str(choice)) == 'dropout':
            return tf.nn.dropout(input, self.dropout_rate)

    def __look_for_last_checkpoint(self, mode_dir):
        try:
            fr = open(mode_dir + 'checkpoint', "r")
        except:
            return None
        f_line = fr.readline()
        start = f_line.find('"')
        end = f_line.rfind('"')
        return f_line[start + 1:end]

    w = 0
    def get_variable(self, shape, initial='random', dtype=tf.float32,name=None):
        self.w += 1
        if initial==None: initial='random'
        if lower(initial) == 'random':
            initial_weight = tf.random_normal_initializer(stddev=0.05, dtype=dtype)
        elif lower(initial) == 'truncated':
            initial_weight = tf.truncated_normal_initializer(stddev=0.05, dtype=dtype)
        elif lower(initial) == 'uniform':
            initial_weight = tf.random_uniform_initializer()
        elif lower(initial) == 'xavier':
            initial_weight = tf.contrib.layers.xavier_initializer()
        elif lower(initial) == 'xavier_conv2d':
            initial_weight = tf.contrib.layers.xavier_initializer_conv2d()
        else:
            initial_weight = initial
        if name!=None:
            tf.get_variable(name, shape=shape, initializer=initial_weight, dtype=dtype)
        return tf.get_variable('weight_' + str(self.w), shape=shape, initializer=initial_weight, dtype=dtype)

    def bias_variable(self, shape,value=None):
        if value==None:
            initial = tf.constant(0.001, shape=shape)
        else:
            initial = tf.constant(value, shape=shape)
        return tf.Variable(initial)

    def conv2d(self, input, kernel_size, strides, padding='SAME', initial='random', groups=1, with_bias=True):
        if groups == 1:
            W = self.get_variable(kernel_size, initial)
            conv = tf.nn.conv2d(input, W, strides, padding=padding, )
            if with_bias:
                return tf.nn.bias_add(conv, self.bias_variable([kernel_size[3]]))
            return conv
        else:
            convolve = lambda i, k: tf.nn.conv2d(i, k, strides=strides, padding=padding)
            W = self.get_variable(kernel_size, initial)
            input_groups = tf.split(axis=3, num_or_size_splits=groups, value=input)
            weight_groups = tf.split(axis=3, num_or_size_splits=groups, value=W)
            output_groups = [convolve(i, k) for i, k in zip(input_groups, weight_groups)]
            conv = tf.concat(axis=3, values=output_groups)
            if with_bias:
                tf.nn.bias_add(conv, self.bias_variable([kernel_size[3]]))
            return conv
    def flatten(self,input):
        op_shape = input.get_shape().as_list()[1:]
        dim=1
        for value in op_shape:
            dim =dim*value
        return tf.reshape(input, [-1, dim])
    def avg_pool(self, input, size, strides, padding='SAME'):
        return tf.nn.avg_pool(input, size, strides, padding)

    def max_pool(self, input, size, strides, padding='SAME'):
        return tf.nn.max_pool(input, size, strides, padding)

    def global_avg_pool(self,input, dim=[1, 2]):
        assert input.get_shape().ndims == 4
        return tf.reduce_mean(input, dim)

    def get_loss(self,logits, labels, loss_type='softmax'):
        if lower(loss_type) == "softmax":
            cross_entropy = tf.reduce_mean(
                tf.nn.softmax_cross_entropy_with_logits(labels=labels,logits=logits,name='Cross_entropy'))
        elif lower(loss_type) == "hinge":
            cross_entropy = tf.reduce_mean(tf.losses.hinge_loss(logits=logits, labels=labels))
        elif lower(loss_type) == "huber":
            cross_entropy = tf.reduce_mean(tf.losses.huber_loss(labels=labels, predictions=logits))
        elif lower(loss_type) == "log":
            cross_entropy = tf.reduce_mean(tf.losses.log_loss(labels=labels, predictions=logits))
        elif lower(loss_type) == "absolute":
            cross_entropy = tf.reduce_mean(tf.losses.absolute_difference(labels=labels, predictions=logits))
        elif lower(loss_type) == "mse":
            cross_entropy = tf.losses.mean_squared_error(labels=labels, predictions=logits)
        elif lower(loss_type) == "mpse":
            cross_entropy = tf.losses.mean_pairwise_squared_error(labels=labels, predictions=logits)
        elif lower(loss_type) == "sigmoid":
            cross_entropy = tf.losses.sigmoid_cross_entropy(labels, logits)
        elif lower(loss_type) == "binary_crossentropy":
            cross_entropy = tf.reduce_mean(tf.keras.losses.binary_crossentropy(logits, labels))
        return cross_entropy

    def get_regularization(self, input_loss, loss_type='l2', regularization_coefficient=0.0001):
        if loss_type == 'l2':
            beta = tf.constant(regularization_coefficient)
            regularized_loss = input_loss + beta * tf.add_n(
                [tf.nn.l2_loss(var) for var in tf.trainable_variables()], 'L2_regurlization')
            return regularized_loss
        if loss_type == 'l1':
            l1_regularizer = tf.contrib.layers.l1_regularizer(scale=regularization_coefficient, scope=None)
            weights = tf.trainable_variables()
            regularization_penalty = tf.contrib.layers.apply_regularization(l1_regularizer, weights)
            regularized_loss = input_loss + regularization_penalty
            return regularized_loss
        if loss_type == 'elastic_net':
            l1_regularizer = tf.contrib.layers.l1_regularizer(scale=regularization_coefficient[0], scope=None)
            l2_regularizer = tf.contrib.layers.l2_regularizer(scale=regularization_coefficient[1], scope=None)
            weights = tf.trainable_variables()
            l1_regularization_penalty = tf.contrib.layers.apply_regularization(l1_regularizer, weights)
            l2_regularization_penalty = tf.contrib.layers.apply_regularization(l2_regularizer, weights)
            regularized_loss = input_loss + l1_regularization_penalty + l2_regularization_penalty
            return regularized_loss

    def get_hidden_layer(self, input, size=50, activation='relu', initializer='random', dtype=tf.float32):
        node_shape = input.get_shape().as_list()[1:]
        weight = self.get_variable([node_shape[0], size], initializer, dtype)
        bias = self.get_variable([1, size], initializer, dtype)
        output = tf.add(tf.matmul(input, weight), bias)
        if isinstance(activation, int):
            output = self.get_activation_function(output, activation)
        elif isinstance(activation, str):
            output = self.get_activation_function(output, activation)
        elif isinstance(activation, list):
            for a in activation:
                output = self.get_activation_function(output, a)
        elif activation==None:
            output=self.get_activation_function(output,'relu')
        return output
    def get_n_hidden_layers(self,input,hidden_sizes=None,activation_function_list=None,initializer=None):
        try:
            no_of_layers = len(hidden_sizes)
        except:
            no_of_layers = 0
        for i in range(no_of_layers):
            with tf.name_scope('Hidden_Layer_' + str(i + 1)):
                if i==0:
                    output=self.get_hidden_layer(input,hidden_sizes[i],activation=activation_function_list[i],initializer=initializer)
                else:
                    output = self.get_hidden_layer(output, hidden_sizes[i], activation=activation_function_list[i],initializer=initializer)
        return output

    def batch_normalization(self,input,is_training):
        return tf.contrib.layers.batch_norm(input, scale=True, is_training=is_training,updates_collections=None)

    __dense = 1
    def conv2d_dense_block(self, input, is_training, kernel=[3, 3, 16, 12], strides=[1, 1, 1, 1], layers=5):
        current = input
        features = kernel[2]
        for _ in range(layers):
            with tf.variable_scope('Bottleneck_layer_' + str(self.__dense)) as scope:
                tmp = tf.contrib.layers.batch_norm(current, scale=True, is_training=is_training,
                                                   updates_collections=None)
                tmp = tf.nn.relu(tmp)
                tmp = self.conv2d(tmp, [kernel[0], kernel[0], features, kernel[3]], strides=strides, padding='SAME',
                                  initial='xavier', with_bias=False)
                tmp = tf.nn.dropout(tmp, self.dropout_rate)
                current = tf.concat([current, tmp], 3)
                features += kernel[3]
                self.__dense += 1
        return current

    def residual_block(self,input,is_training,output_channel,kernel=3,stride=1,first_block=False,padding_option=0,pad_or_conv=False):
        original_shape = input.get_shape().as_list()[1:]
        original_input=input

        if first_block:
            input = self.conv2d(input, kernel_size=[kernel, kernel, original_shape[2], output_channel],
                                strides=[1, stride, stride, 1], padding='SAME', initial='xavier', with_bias=False)
        else:
            input = self.batch_normalization(input,is_training)
            input = tf.nn.relu(input)
            input = self.conv2d(input, kernel_size=[kernel,kernel,original_shape[2],output_channel],
                                strides=[1, stride, stride, 1], padding='SAME', initial='xavier',with_bias=False)

        input = self.batch_normalization(input,is_training)
        input = tf.nn.relu(input)
        input = self.conv2d(input, kernel_size=[kernel, kernel, output_channel, output_channel], strides=[1, 1, 1, 1],
                            padding='SAME', initial='xavier', with_bias=False)

        if stride != 1:
            original_input = self.avg_pool(original_input, [1, stride, stride, 1], [1, stride, stride, 1], 'VALID')
        input_shape = input.get_shape().as_list()[1:]

        if original_shape[2] != input_shape[2] and original_shape[2] < input_shape[2]:
            if pad_or_conv == True:
                original_input = self.conv2d(original_input, [1, 1, original_shape[2], input_shape[2]], [1, 1, 1, 1],
                                             with_bias=False)
            else:
                original_input = tf.pad(original_input,
                                        [[padding_option, padding_option], [padding_option, padding_option],
                                         [padding_option, padding_option],
                                         [(input_shape[2] - original_shape[2]) // 2,
                                          (input_shape[2] - original_shape[2]) // 2]])
        elif original_shape[2] != input_shape[2] and original_shape[2] > input_shape[2]:
            original_input = self.conv2d(original_input, [1, 1, original_shape[2], input_shape[2]], [1, 1, 1, 1],
                                     with_bias=False)
        return input + original_input

    def residual_bottleneck_block(self,input,is_training, output_channel,kernel=3,stride=1,first_block=False,padding_option=0,pad_or_conv=False):
        original_shape = input.get_shape().as_list()[1:]
        original_input=input

        if first_block:
            input = self.conv2d(input, kernel_size=[1,1, original_shape[2], output_channel/4],
                                strides=[1, stride, stride, 1], padding='SAME', initial='xavier', with_bias=False)
        else:
            input = self.batch_normalization(input,is_training)
            input = tf.nn.relu(input)
            input = self.conv2d(input, kernel_size=[1,1,original_shape[2],output_channel/4],
                                strides=[1, stride, stride, 1], padding='SAME', initial='xavier',with_bias=False)

        input = self.batch_normalization(input,is_training)
        input = tf.nn.relu(input)
        input = self.conv2d(input, kernel_size=[kernel, kernel, output_channel/4, output_channel/4], strides=[1, 1, 1, 1],
                            padding='SAME', initial='xavier', with_bias=False)

        input = self.batch_normalization(input,is_training)
        input = tf.nn.relu(input)
        input = self.conv2d(input, kernel_size=[kernel, kernel, output_channel/4, output_channel], strides=[1, 1, 1, 1],
                            padding='SAME', initial='xavier', with_bias=False)

        if stride!=1:
            original_input = self.avg_pool(original_input, [1, stride, stride, 1], [1, stride, stride, 1], 'VALID')
        input_shape = input.get_shape().as_list()[1:]

        if original_shape[2] != input_shape[2] and original_shape[2] < input_shape[2]:
            if pad_or_conv==True:
                original_input = self.conv2d(original_input, [1, 1, original_shape[2], input_shape[2]], [1, 1, 1, 1],with_bias=False)
            else:
                original_input = tf.pad(original_input, [[padding_option, padding_option], [padding_option,padding_option], [padding_option, padding_option],
                             [(input_shape[2] - original_shape[2]) // 2, (input_shape[2] - original_shape[2]) // 2]])
        elif original_shape[2] != input_shape[2] and original_shape[2] > input_shape[2]:
            original_input = self.conv2d(original_input, [1, 1, original_shape[2], input_shape[2]], [1, 1, 1, 1],with_bias=False)

        return input + original_input

    def residual_wide_block(self,input,is_training,output_channel,kernel=3,stride=1,first_block=False,padding_option=0,pad_or_conv=False):
        original_shape = input.get_shape().as_list()[1:]
        original_input=input
        if first_block:
            input = self.conv2d(input, kernel_size=[kernel, kernel, original_shape[2], output_channel],
                                strides=[1, stride, stride, 1], padding='SAME', initial='xavier', with_bias=False)
        else:
            input = self.batch_normalization(input,is_training)
            input = tf.nn.relu(input)
            input = self.conv2d(input, kernel_size=[kernel,kernel,original_shape[2],output_channel],
                                strides=[1, stride, stride, 1], padding='SAME', initial='xavier',with_bias=False)

        input=tf.nn.dropout(input,self.dropout_rate)

        input = self.batch_normalization(input,is_training)
        input = tf.nn.relu(input)
        input = self.conv2d(input, kernel_size=[kernel, kernel, output_channel, output_channel], strides=[1, 1, 1, 1],
                            padding='SAME', initial='xavier', with_bias=False)

        if stride != 1:
            original_input = self.avg_pool(original_input, [1, stride, stride, 1], [1, stride, stride, 1], 'VALID')
        input_shape = input.get_shape().as_list()[1:]

        if original_shape[2] != input_shape[2] and original_shape[2] < input_shape[2]:
            if pad_or_conv == True:
                original_input = self.conv2d(original_input, [1, 1, original_shape[2], input_shape[2]], [1, 1, 1, 1],
                                             with_bias=False)
            else:
                original_input = tf.pad(original_input,
                                        [[padding_option, padding_option], [padding_option, padding_option],
                                         [padding_option, padding_option],
                                         [(input_shape[2] - original_shape[2]) // 2,
                                          (input_shape[2] - original_shape[2]) // 2]])
        elif original_shape[2] != input_shape[2] and original_shape[2] > input_shape[2]:
            original_input = self.conv2d(original_input, [1, 1, original_shape[2], input_shape[2]], [1, 1, 1, 1],
                                         with_bias=False)

        return input + original_input

    def densenet(self, x, is_training, no_of_blocks=3, block_layers=7, first_conv_op_channel=16, block_op_channel=12,
                 kernal_size=3):

        strides = {'1': [1, 1, 1, 1], '2': [1, 2, 2, 1], '3': [1, 3, 3, 1], '4': [1, 4, 4, 1], '8': [1, 8, 8, 1]}
        pool_win_size = {'1': [1, 1, 1, 1], '2': [1, 2, 2, 1], '3': [1, 3, 3, 1], '4': [1, 4, 4, 1], '8': [1, 8, 8, 1]}

        kernel = [kernal_size, kernal_size, self.image_shape[2], first_conv_op_channel]
        conv = self.conv2d(x, kernel, strides['1'], 'SAME', initial='xavier', with_bias=False)
        if isinstance(block_layers, int):
            with tf.variable_scope('Dense_Block_1') as scope:
                kernel = [kernal_size, kernal_size, first_conv_op_channel, block_op_channel]
                conv = self.conv2d_dense_block(conv, is_training, kernel, layers=block_layers)
                op_channel = first_conv_op_channel + block_layers * block_op_channel
            for _ in range(1, no_of_blocks):
                with tf.variable_scope('transition_layer_' + str(_ - 1)) as scope:
                    conv = tf.contrib.layers.batch_norm(conv, scale=True, is_training=is_training,
                                                        updates_collections=None)
                    conv = tf.nn.relu(conv)
                    kernel = [kernal_size, kernal_size, op_channel, op_channel]
                    conv = self.conv2d(conv, kernel, strides=[1, 1, 1, 1], padding='SAME', initial='xavier',
                                       with_bias=False)
                    conv = tf.nn.dropout(conv, self.dropout_rate)
                    conv = self.avg_pool(conv, pool_win_size['2'], strides['2'], 'VALID')
                with tf.variable_scope('Dense_Block_' + str(_)) as scope:
                    kernel = [kernal_size, kernal_size, op_channel, block_op_channel]
                    conv = self.conv2d_dense_block(conv, is_training, kernel, layers=block_layers)
                    op_channel += block_layers * block_op_channel
        elif isinstance(block_layers, list):
            no_of_blocks = len(block_layers)

            with tf.variable_scope('Dense_Block_1') as scope:
                kernel = [kernal_size, kernal_size, first_conv_op_channel, block_op_channel]
                conv = self.conv2d_dense_block(conv, is_training, kernel, layers=block_layers[0])
                op_channel = first_conv_op_channel + block_layers[0] * block_op_channel

            for _ in range(1, no_of_blocks):
                with tf.variable_scope('transition_layer_' + str(_)) as scope:
                    conv = tf.contrib.layers.batch_norm(conv, scale=True, is_training=is_training,
                                                        updates_collections=None)
                    conv = tf.nn.relu(conv)
                    kernel = [kernal_size, kernal_size, op_channel, op_channel]
                    conv = self.conv2d(conv, kernel, strides=[1, 1, 1, 1], padding='SAME', initial='xavier',
                                       with_bias=False)
                    conv = tf.nn.dropout(conv, self.dropout_rate)
                    conv = self.avg_pool(conv, pool_win_size['2'], strides['2'], 'VALID')
                with tf.variable_scope('Dense_Block_' + str(_ + 1)) as scope:
                    kernel = [kernal_size, kernal_size, op_channel, block_op_channel]
                    conv = self.conv2d_dense_block(conv, is_training, kernel, layers=block_layers[_])
                    op_channel += block_layers[_] * block_op_channel
        with tf.variable_scope('Global_Average_Pooling') as scope:
            conv = tf.contrib.layers.batch_norm(conv, scale=True, is_training=is_training, updates_collections=None)
            conv = tf.nn.relu(conv)
            conv = self.avg_pool(conv, pool_win_size['8'], strides['8'], 'VALID')

        with tf.variable_scope('Flatten_layer') as scope:
            op_shape = conv.get_shape().as_list()[1:]
            dim = op_shape[0] * op_shape[1] * op_shape[2]
            conv = tf.reshape(conv, [-1, dim])

        with tf.variable_scope('Output_layer') as scope:
            conv = self.get_hidden_layer(conv, self.no_of_classes, activation='none', initializer='xavier')

        return conv

    def densenet121(self,input,is_training):
        return self.densenet(input,is_training, block_layers=[6, 12, 24, 16], first_conv_op_channel=64, block_op_channel=32,
                                   kernal_size=7)

    def densenet161(self, input, is_training):
        return self.densenet(input, is_training, block_layers=[6, 12, 36, 24], first_conv_op_channel=96, block_op_channel=48,
                                   kernal_size=7)

    def densenet169(self, input, is_training):
        return self.densenet(input, is_training, block_layers=[6, 12, 32, 32], first_conv_op_channel=64, block_op_channel=32,
                                   kernal_size=7)
    def mydensenet(self,input,is_training):
        return self.densenet(input, is_training, no_of_blocks=3, block_layers=[7,7,7], first_conv_op_channel=32,
                 block_op_channel=16, kernal_size=3)

    def resnet_without_bottleneck(self, input, is_training,layer_from_2=[2,2,2,2],first_kernel=7,first_stride=2,first_pool=True,stride=2):

        input_shape = input.get_shape().as_list()[1:]
        conv=self.conv2d(input,[first_kernel,first_kernel,input_shape[2],64],[1,first_stride,first_stride,1])
        if first_pool:
            conv=self.max_pool(conv, [1, 3, 3, 1], [1, 2, 2, 1])

        for i in range(layer_from_2[0]):
            conv=self.residual_block(conv,is_training,64,kernel=3,first_block=True,stride=stride)

        for i in range(layer_from_2[1]):
            conv=self.residual_block(conv,is_training,128,kernel=3,first_block=True,stride=stride)

        for i in range(layer_from_2[2]):
            conv=self.residual_block(conv,is_training,256,kernel=3,first_block=True,stride=stride)

        for i in range(layer_from_2[3]):
            conv=self.residual_block(conv,is_training,512,kernel=3,first_block=True,stride=stride)

        with tf.variable_scope('unit'):
            conv = self.batch_normalization(conv,is_training)
            conv = tf.nn.relu(conv)
            conv = self.global_avg_pool(conv)
        with tf.variable_scope('logit'):
            conv = self.get_hidden_layer(conv,self.no_of_classes,'none')
        return conv

    def resnet_with_bottleneck(self,input,is_training,layer_from_2=[3,4,6,3],first_kernel=7,first_stride=2,first_pool=True,stride=2):

        input_shape = input.get_shape().as_list()[1:]
        conv=self.conv2d(input,[first_kernel,first_kernel,input_shape[2],64],[1,first_stride,first_stride,1])
        if first_pool:
            conv=self.max_pool(conv, [1, 3, 3, 1], [1, 2, 2, 1])

        for i in range(layer_from_2[0]):
            conv=self.residual_bottleneck_block(conv,is_training,256,kernel=3,first_block=True,stride=stride)

        for i in range(layer_from_2[1]):
            conv=self.residual_bottleneck_block(conv,is_training,512,kernel=3,first_block=True,stride=stride)

        for i in range(layer_from_2[2]):
            conv=self.residual_bottleneck_block(conv,is_training,1024,kernel=3,first_block=True,stride=stride)

        for i in range(layer_from_2[3]):
            conv=self.residual_bottleneck_block(conv,is_training,2048,kernel=3,first_block=True,stride=stride)
        with tf.variable_scope('unit'):
            conv = self.batch_normalization(conv,is_training)
            conv = tf.nn.relu(conv)
            conv = self.global_avg_pool(conv)
        with tf.variable_scope('logit'):

            conv = self.get_hidden_layer(conv,self.no_of_classes,'none')
        return conv

    def resnet(self,x,is_training):
        return self.resnet_with_bottleneck(x, is_training,first_pool=False,layer_from_2=[2,2,2,2],first_stride=1,stride=1)

    def resnet18(self,input,is_training,first_kernel=7,first_stride=2,first_pool=True,stride=2):
        return self.resnet_without_bottleneck(input,is_training,[2,2,2,2],first_kernel,first_stride,first_pool,stride)

    def resnet32(self,input,is_training,first_kernel=7,first_stride=2,first_pool=True,stride=2):
        return self.resnet_without_bottleneck(input,is_training,[3,4,6,3],first_kernel,first_stride,first_pool,stride)

    def resnet50(self,input,is_training,first_kernel=7,first_stride=2,first_pool=True,stride=2):
        return self.resnet_with_bottleneck(input,is_training,[3,4,6,3],first_kernel,first_stride,first_pool,stride)

    def resnet101(self,input,is_training,first_kernel=7,first_stride=2,first_pool=True,stride=2):
        return self.resnet_with_bottleneck(input,is_training,[3,4,23,3],first_kernel,first_stride,first_pool,stride)

    def resnet152(self,input,is_training,first_kernel=7,first_stride=2,first_pool=True,stride=2):
        return self.resnet_with_bottleneck(input,is_training,[3,4,36,3],first_kernel,first_stride,first_pool,stride)

    def train(self,train,val_data,max_keep=100):
        init = tf.global_variables_initializer()
        self.session.run(init)

        saver = tf.train.Saver(max_to_keep=max_keep)
        if self.model_restore == True and self.working_dir!= None:
            name = self.__look_for_last_checkpoint(self.working_dir + "/model/")
            if name is not None:
                saver.restore(self.session, self.working_dir + "/model/" + name)
                print('Model Succesfully Loaded : ', name)
        if self.working_dir != None:
            merged = tf.summary.merge_all()
            train_writer = tf.summary.FileWriter(self.working_dir + '/train', self.session.graph)
            test_writer = tf.summary.FileWriter(self.working_dir + '/test')
        test_result = []
        train_result = []

        for epoch in range(1,self.epochs+1):
            ind_list = [i for i in range(len(train['x']))]
            random.shuffle(ind_list)
            train['x']=train['x'][ind_list]
            train['y']=train['y'][ind_list]
            epoch_loss = 0
            acc = 0
            i = 0
            batch_iteration = 0
            while i < len(train['x']):
                start = i
                end = i + self.batch_size
                if (end > len(train['x'])): end = len(train['x'])
                batch_x = train['x'][start:end]
                batch_y = train['y'][start:end]
                if self.working_dir != None:
                    summary, _, loss,batch_acc= self.session.run([merged, self.optimizer,self.cross_entropy,self.accuracy],
                                                              feed_dict={self.x: batch_x, self.y: batch_y,self.lr:self.learning_rate,self.is_train:True})
                else:
                    _, loss,batch_acc= self.session.run([self.optimizer, self.cross_entropy,self.accuracy],
                                                     feed_dict={self.x: batch_x,self.y: batch_y,self.lr: self.learning_rate,self.is_train:True})
                epoch_loss += loss
                acc += batch_acc
                batch_iteration += 1
                i += self.batch_size
            if self.working_dir != None:
                train_writer.add_summary(summary, epoch)
            train_result.append([epoch, epoch_loss/batch_iteration, acc / batch_iteration])
            if val_data != None:
                epoch_loss = 0
                acc = 0
                i = 0
                batch_iteration = 0
                while i < len(val_data['x']):
                    start = i
                    end = i + self.batch_size
                    if (end > len(val_data['x'])): end = len(val_data['x'])
                    batch_x = val_data['x'][start:end]
                    batch_y = val_data['y'][start:end]
                    if self.working_dir != None:
                        summary,loss,batch_acc= self.session.run([merged,self.cross_entropy,self.accuracy],
                                                              feed_dict={self.x: batch_x, self.y: batch_y,self.lr:self.learning_rate,self.is_train:False})
                    else:
                        loss,batch_acc= self.session.run([self.cross_entropy,self.accuracy],
                                                     feed_dict={self.x: batch_x,self.y: batch_y,self.lr: self.learning_rate,self.is_train:False})
                    epoch_loss += loss
                    acc += batch_acc
                    batch_iteration += 1
                    i += self.batch_size
                if self.working_dir != None:
                    test_writer.add_summary(summary, epoch)
                test_result.append([epoch, epoch_loss/batch_iteration, acc / batch_iteration])

                print("Training:", train_result[len(train_result) - 1], "Val:", test_result[len(test_result) - 1])
            else:
                print("Training :", train_result[len(train_result) - 1])

            if self.working_dir != None:
                save_path = saver.save(self.session, self.working_dir + "/model/" + 'model',global_step=epoch)
        print('Training Succesfully Complete')
        self.test_result=test_result
        self.train_result=train_result

    def check_restore(self):
        saver = tf.train.Saver()
        print(self.working_dir+'/model/')
        try:
            saver.restore(self.session, tf.train.latest_checkpoint(self.working_dir+'/model/'))
            return True
        except:
            return False

    def predict(self,test):
        saver = tf.train.Saver()
        print(self.working_dir+'/model/')
        saver.restore(self.session, tf.train.latest_checkpoint(self.working_dir+'/model/'))
        merged = tf.summary.merge_all()
        if 'x' in test and test['x'].shape[0] > 0:
            i = 0
            iteration = 0
            acc = 0
            test_prediction=[]
            j=0
            while i < len(test['x']):
                start = i
                end = i + self.batch_size
                if (end > len(test['x'])): end = len(test['x'])
                batch_x = test['x'][start:end]
                if 'y' in test and test['y'].shape[0] > 0:
                    batch_y = test['y'][start:end]
                    pred,batch_acc = self.session.run([self.prediction,self.accuracy], feed_dict={self.x: batch_x, self.y: batch_y,self.is_train:False})
                    acc += batch_acc
                else:
                    pred= self.session.run([self.prediction], feed_dict={self.x: batch_x,self.is_train:False})
                iteration += 1
                i += self.batch_size
                if isinstance(pred,list):
                    test_prediction+=pred[0].tolist()
                else:
                    test_prediction += pred.tolist()
            if 'y' in test and test['y'].shape[0] > 0:
                return np.array(test_prediction), acc/iteration
            else:
                return np.array(test_prediction)
