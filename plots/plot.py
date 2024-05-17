import matplotlib.pyplot as plt
import re
import numpy as np

# Function to parse the file and return a dictionary of data
def parse_results_file(filename):
    results = {}
    with open(filename, 'r') as file:
        lines = file.readlines()
        
        current_k = None
        
        for line in lines:
            k_match = re.match(r"K: (\d+), W: (\d+)", line)
            if k_match:
                current_k = int(k_match.group(1))
                current_w = int(k_match.group(2))
                if current_k not in results:
                    results[current_k] = {'density': [], 'query_time': [], 'w': []}
                results[current_k]['w'].append(current_w)
                
            density_match = re.match(r"Density: ([0-9.]+)", line)
            if density_match:
                density = float(density_match.group(1))
                results[current_k]['density'].append(density)
            query_time_match = re.match(r"Avg. Query Time: ([0-9.]+) ms", line)
            if query_time_match:
                query_time = float(query_time_match.group(1))
                results[current_k]['query_time'].append(query_time)
    return results

# Function to plot the data
def K_plot(results, fname):
    plt.figure(figsize=(10, 6))
    plt.rcParams.update({'font.size': 20})

    # plt.yscale("log")
    
    for k, data in results.items():
        if k == 1:
            continue
        plt.plot(data['density'], data['query_time'], marker='o', label=f"K: {k}")

    if 1 in results:
        k = 1
        data = results[1]
        plt.plot(data['density'], data['query_time'], marker='o', label="Baseline")

    plt.xlabel("Density")
    plt.ylabel("Query Time (ms)")
    # plt.title("Query Time vs Density for Different K Values")
    plt.legend()
    plt.grid(True)
    plt.tight_layout()
    plt.savefig(fname)


# Function to plot the data
def comp_plot(results_dict, k, fname):
    plt.figure(figsize=(10, 6))
    plt.rcParams.update({'font.size': 20})


    for method, results in results_dict.items():
        plt.plot(results[k]['w'], [1 / x for x in results[k]['density']], marker='o', label=method)


    plt.xlabel("W")
    plt.ylabel("Sparsity")
    # plt.title("Sparsity vs W for Different Ordering Methods")
    plt.legend()
    plt.grid(True)
    plt.tight_layout()
    plt.savefig(fname)

def comp_bar_plot(results_dict, k, fname):
    plt.figure(figsize=(10, 6))
    plt.rcParams.update({'font.size': 20})
    
    # Extract the list of W values
    w_values = sorted(list({w for results in results_dict.values() for w in results[k]['w']}))
    
    # Determine the number of methods
    num_methods = len(results_dict)
    
    # Bar width
    bar_width = 0.8 / num_methods
    
    # X positions for each group of bars
    x = np.arange(len(w_values))
    
    for i, (method, results) in enumerate(results_dict.items()):
        # Calculate bar positions for this method
        bar_positions = x + i * bar_width
        
        # Filter densities based on the w_values
        sparsities = [1 / results[k]['density'][results[k]['w'].index(w)] if w in results[k]['w'] else 0 for w in w_values]
        
        # Plot the bars
        plt.bar(bar_positions, sparsities, width=bar_width, label=method)
    
    plt.xlabel("W")
    plt.ylabel("Sparsity")
    # plt.title("Sparsity vs W for Different Ordering Methods")
    plt.xticks(x + bar_width * (num_methods - 1) / 2, w_values)
    plt.legend()
    plt.gca().yaxis.grid(True, linestyle='--')
    plt.tight_layout()
    plt.savefig(fname)


# Function to plot the data
def comp_lcp_plot(results, results_lcp, k, fname):
    plt.figure(figsize=(10, 6))
    plt.rcParams.update({'font.size': 20})

    plt.plot(results[k]['w'], results[k]['query_time'], marker='o', label='w/o prefix lookup')
    plt.plot(results_lcp[k]['w'], results_lcp[k]['query_time'], marker='o', label='w prefix lookup')


    plt.xlabel("W")
    plt.ylabel("Query Time (ms)")
    # plt.title("Comp vs W for Different Ordering Methods")
    plt.legend()
    plt.grid(True)
    plt.tight_layout()
    plt.savefig(fname)

# Main code
results_lexi = parse_results_file('results_lexi.txt')
# K_plot(results_lexi, "plot_lexi.png")

results_rand = parse_results_file('results_rand.txt')
K_plot(results_rand, "plot_k_rand.png")

results_lexi_rev = parse_results_file('results_lexi_rev.txt')


# comp_plot({'lexi': results_lexi, 'lexi_rev': results_lexi_rev, 'rand': results_rand}, 3, 'comparison.png')
comp_bar_plot({'lexi': results_lexi, 'lexi_rev': results_lexi_rev, 'rand': results_rand}, 3, 'comparison_bar.png')

results_rand_lcp = parse_results_file('results_rand_lcp.txt')
comp_lcp_plot(results_rand, results_rand_lcp, 3, 'comp_lcp.png')
