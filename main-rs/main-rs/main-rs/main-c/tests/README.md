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
<!-- 
## 🧪 GitHub Actions | CI Pipeline

**Integrarea Continuă** este o parte esențială a procesului de testare.

În acest proiect, **GitHub Actions** rulează automat testele
ori de câte ori se face *push* în repository sau se deschide un *pull request*.

Workflow-ul este definit în următorul fișier:
[../.github/workflows/CI-tests.yml](./../.github/workflows/CI-tests.yml)
-->
