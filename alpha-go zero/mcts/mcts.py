import numpy as np

class MCTSNode:
    def __init__(self, parent, prior_prob):
        self.parent = parent
        self.children = {}
        self.visit_count = 0
        self.total_value = 0
        self.prior_prob = prior_prob

    def expand(self, action_priors):
        for action, prob in action_priors:
            if action not in self.children:
                self.children[action] = MCTSNode(self, prob)

    def select(self, c_puct):
        return max(self.children.items(), key=lambda act_node: act_node[1].get_value(c_puct))

    def update(self, value):
        self.visit_count += 1
        self.total_value += value

    def get_value(self, c_puct):
        u = c_puct * self.prior_prob * np.sqrt(self.parent.visit_count) / (1 + self.visit_count)
        return self.total_value / (1 + self.visit_count) + u
