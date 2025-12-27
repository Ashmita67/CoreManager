import pandas as pd
from sklearn.linear_model import LinearRegression

data = pd.read_csv("data/usage_log.csv")
X = data[["core0", "core1", "core2"]]
y = data["core3"]
model = LinearRegression().fit(X, y)
print(model.coef_, model.intercept_)

# Save model parameters
with open("data/model.txt", "w") as f:
    for c in model.coef_:
        f.write(f"{c}\\n")
    f.write(f"{model.intercept_}\\n")
