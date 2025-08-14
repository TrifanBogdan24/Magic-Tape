# ✅🔁 Testare Automată

- Testarea implementării:
```sh
cd tests/
chmod +x checker.sh
./checker.sh
```

- Testarea coding-style-ului:
```sh
# Din rădăcina repository-ului:
chmod +x tests/coding-style/cs.sh
./tests/coding-style/cs.sh src/*
```

## 🧪 GitHub Actions | CI Pipeline

**Integrarea Continuă** este o parte esențială a procesului de testare.

În acest proiect, **GitHub Actions** rulează automat testele
ori de câte ori se face *push* în repository sau se deschide un *pull request*.

Workflow-ul este definit în următorul fișier:
[../.github/workflows/CI-tests.yml](./../.github/workflows/CI-tests.yml)


## 🌃 Overnight Testing

Testele nu numai că sunt rulate la fiecare **commit**/**pull request**,  
dar, mai mult decât atât, am extins workflow-ul de CI  
să includă o rulare **programată în fiecare noapte** (`cron job`),  
care execută integral testele în **GitHub Actions**.

> În **DevOps**, acest tip de execuție programată poartă numele de *"overnight testing"*.



```yml
on:
  schedule:
    # Overnight: run tests every day at 21:30 UTC
    # Usage: cron <minute> <hour> <day-of-month> <day-of-week (sunday=0)>
    - cron: "30 21 * * *"
```

Pentru a distinge clar rulările programate de cele on-demand (**commit**/**PR**),  
workflow-ul **overnight** utilizează un **label** dedicat
vizibil în lista job-urilor **GitHub Actions**.


```yml
run-name: >
  ${{ github.event_name == 'schedule'
      && '🌃 Running Overnight Tests'
      || (
        github.event_name == 'push'
        && format(github.event.head_commit.message)
        || (
          github.event_name == 'pull_request'
          && format(github.event.pull_request.title)
        )
      )
  }}
```
