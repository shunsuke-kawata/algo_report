import glob
import matplotlib.pyplot as plt
import japanize_matplotlib
import numpy as np
import re

dir_rate_path = './learning_rate/*.txt'
file_hidden_path ='./learning_hidden/hidden_result.txt'

def get_file_list(path):
    files = glob.glob(path)
    return files

def plot_learning_rate_graph(file):
    regex_pattern = re.escape(dir_rate_path).replace(r'\*', r'(.*)')
    match = re.match(regex_pattern, file)
    if match:
        learning_rate = match.group(1)
        float_str  = str(float(learning_rate))
    else:
        print("一致する文字列が見つかりませんでした。")
    with open(file, 'r') as f:
        lines = f.readlines()
    data = []
    for line in lines:
        # 行をスペースまたはタブで分割し、数値を浮動小数点数として取得
        tmp = line.strip().split()
        tmp[0] = int(tmp[0])
        tmp[1] = float(tmp[1])
        data.append(tmp)
    
    #dataの作成終了
    tmp_np_array = np.array(data).T
    max_value = np.max(tmp_np_array[1])
    
    y_tick = [0]
    for j in range(1,int(max_value*100),10):
        y_tick.append(round(float(j)/100,2))
    y_tick.append(round(max_value,2))
    
    transfered_list= tmp_np_array.tolist()
    plt.figure(figsize=(8.4,5.8))
    plt.title(f"学習率 {float_str}")
    plt.xlabel("学習ステップ")
    plt.ylabel("テストデータの２乗誤差の平均値")
    plt.rcParams['font.family'] = 'Hiragino Sans'
    plt.xticks([i for i in range(0,1201,100)])
    plt.yticks(y_tick)
    # plt.tick_params(labelbottom=False, labelleft=False, labelright=False, labeltop=False)
    plt.scatter(transfered_list[0],transfered_list[1],s=3,marker='.')
    output_path = './out/'+float_str+'.png'
    plt.savefig(output_path) 
  
def plot_learning_hidden_graph(file):
    with open(file, 'r') as f:
        lines = f.readlines()
    data = []
    for line in lines:
        # 行をスペースまたはタブで分割し、数値を浮動小数点数として取得
        tmp = line.strip().split()
        tmp[0] = int(tmp[0])
        tmp[1] = float(tmp[1])
        data.append(tmp)
    tmp_np_array = np.array(data).T
    max_value = np.max(tmp_np_array[1])
    
    y_tick = [0]
    for j in range(1,int(max_value*100),10):
        y_tick.append(round(float(j)/100,2))
    y_tick.append(round(max_value,2))
    transfered_list= tmp_np_array.tolist()
    plt.figure(figsize=(8.4,5.8))
    plt.title("学習終了後のテストデータの２乗誤差の平均値")
    plt.xlabel("HIDDEN_NUMBER")
    plt.ylabel("テストデータの２乗誤差の平均値")
    plt.rcParams['font.family'] = 'Hiragino Sans'
    plt.xticks(transfered_list[0])
    plt.yticks(y_tick)
    plt.scatter(transfered_list[0],transfered_list[1],s=30,marker='.')
    output_path = './out/hidden_result.png'
    plt.savefig(output_path) 
    
def main():
    #学習率ごとの結果のテキストを取得
    files =get_file_list(dir_rate_path)
    #学習率ごとにグラフを出力
    for file in files:
        plot_learning_rate_graph(file)
    #隠れ層のニューロン数ごとのグラフを出力
    plot_learning_hidden_graph(file_hidden_path)

if __name__ =='__main__':
    main()