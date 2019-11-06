import copy
import gym
from gym import spaces
import pickle
import time
import numpy as np
import re
import subprocess
import os
import traceback
import hashlib
import sys
import os


class SimBanditEnv(gym.Env):
    def __init__(self,env_config):
        self.vec_action_meaning = [1,2,4,8,16]
        self.interleave_action_meaning=[1,2,4,8]
        self.action_space = spaces.Tuple([spaces.Discrete(len(self.vec_action_meaning)),spaces.Discrete(len(self.interleave_action_meaning))])
        #self.action_space = spaces.Box(0.0,1.0,shape=(2,),dtype=np.float32)
        self.obs_len = 384 
        self.continuous = env_config.get('action_type',0)
        self.observation_space = spaces.Box(-1.0,1.,shape=(self.obs_len),dtype = np.float32)
        with open('/home/ameer/Desktop/rlvectorizer/llvm-project/build/rlvectorizer/c-code2vec_embedding/dataset.pkl', 'rb') as f:
            data =  pickle.load(f)
            self.runtimes = data['all_program_runtimes']
            opt_runtimes = data['opt_runtimes']
        with open('/home/ameer/Desktop/rlvectorizer/llvm-project/build/rlvectorizer/c-code2vec_embedding/O3_runtimes.pkl','rb') as f:
            self.o3_runtimes = pickle.load(f)
        with open('/home/ameer/Desktop/rlvectorizer/llvm-project/build/rlvectorizer/c-code2vec_embedding/c_encodings.pkl','rb') as f:
            self.encodings = pickle.load(f)
        self.testkeys =[]
        self.current_file_idx = 0
#        for key in opt_runtimes.keys():
#            o3_key=key.replace('_74296','_22506')
#            o3 = self.o3_runtimes[o3_key]
#            opt = opt_runtimes[key]
#            improv = (o3-opt)*1.0/o3
#            if improv >0.01:
#                self.testkeys.append(key)
        self.testkeys=['garbage24/s6n_64_add_4_74296.c', 'garbage24/s2_4096_2_2_x_out_74296.c', 'garbage24/s12_128_2_2_input_assign_74296.c', 'garbage24/s12n_256_2_2_in1_assign1_74296.c', 'garbage24/s9n_64_64_2_x_74296.c', 'garbage24/s12nn_64_2_2_short_a_assign1_74296.c', 'garbage24/s7n_16384_3_74296.c', 'garbage24/s11_256_mul_4_74296.c', 'garbage24/s13_512_sub_1_74296.c', 'garbage24/s10_8192_add_4_74296.c', 'garbage24/s8_256_4_74296.c', 'garbage24/s7_2048_2_74296.c']
        self.geo=1
        self.datapoints = []
        print(len(self.testkeys))
    def reset(self):
        if self.current_file_idx == 0:
            self.geo=1
        e_key = self.testkeys[self.current_file_idx].replace('_74296','_27483')
        return_e = []
        for val in self.encodings[e_key][0]:
            new_val = val
            if new_val > 1:
                new_val = 1
            if new_val <-1:
                new_val = -1
            return_e.append(new_val)
        return return_e

    def get_VF_IF(self,action):
        if self.continuous == 0:
            action = list(np.reshape(np.array(action),(np.array(action).shape[0],)))
            VF_idx = action[0]
            IF_idx = action[1]
        if self.continuous == 1:
            action = round(action[0]*19.0)
            VF_idx = int(int(action)/len(self.interleave_action_meaning))
            IF_idx = int(int(action)%len(self.interleave_action_meaning))
        if self.continuous == 2:
            VF_idx = int(round(action[0]*(len(self.vec_action_meaning)-1)))
            IF_idx = int(round(action[1]*(len(self.interleave_action_meaning)-1)))

#        print(VF_idx,IF_idx)
        return VF_idx, IF_idx
    def step(self,action):
        VF_idx, IF_idx = self.get_VF_IF(action)
        reward = self.get_reward(VF_idx,IF_idx)

        #print(VF_idx,IF_idx,reward,self.current_file_idx)
        self.current_file_idx = (self.current_file_idx+1)%len(self.testkeys)
        e_key = self.testkeys[self.current_file_idx].replace('_74296','_27483')
        obs = []
        for val in self.encodings[e_key][0]:
            new_val = val
            if new_val > 1:
                new_val = 1
            if new_val <-1:
                new_val = -1
            obs.append(new_val)

        return obs, reward, True, {}

    def get_reward(self,VF_idx,IF_idx):
        key = self.testkeys[self.current_file_idx]
        o3_key = key.replace('_74296','_22506')
        o3 = self.o3_runtimes[o3_key]
        exec_time = self.runtimes[key][VF_idx][IF_idx]
        if(exec_time>=1000000):
            reward= -9
        else:
            reward = (o3-exec_time)*1.0/o3
            self.geo=self.geo*((o3/exec_time)**(1/12))
            #if(self.geo>2.65):
            #    print('improvement',o3/exec_time,'Factors',VF_idx,IF_idx,'geo',self.geo)
            #    print('@@@@@@@@@@@@@@@@@@@@')
        return reward

